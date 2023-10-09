#include "virtual_addr.h"
#include "process/kprocess.h"

namespace stareye {
namespace details {
VirtualAddressFormater LocatePxtBase()
{
    static VirtualAddressFormater PxtBaseCache{0};
    if (PxtBaseCache.quad_part == 0)
    {
        PxtBaseCache.bits.sign_extend = 0xFFFF;
        for (size_t i = PAGE_SIZE / 8; i > 0; i--)
        {
            PxtBaseCache.bits.pxti = i;
            PxtBaseCache.bits.ppti = i;
            PxtBaseCache.bits.pdti = i;
            PxtBaseCache.bits.pti = i;
            auto phyaddr = MmGetPhysicalAddress((PVOID)PxtBaseCache.quad_part);
            if (__readcr3() == phyaddr.QuadPart) {
                goto _success;
            }
        }
        PxtBaseCache.quad_part = 0;
        return { 0 };
    }
_success:
    return PxtBaseCache;
}

VirtualAddressFormater LocatePteBase()
{
    static VirtualAddressFormater PteBaseCache{0};
    if (PteBaseCache.quad_part == 0) {
        PteBaseCache = LocatePxtBase();
        PteBaseCache.bits.pti = 0;
        PteBaseCache.bits.pdti = 0;
        PteBaseCache.bits.ppti = 0;
    }
    return PteBaseCache;
}

VirtualAddressFormater GetPteVirtualAddress(uint64_t vaddr)
{
    auto pte_base = LocatePteBase();
    if (pte_base.quad_part == 0) return { 0 };
    return { ((vaddr >> 9) & VALID_VIRTUAL_ADDRESS_MASK) + pte_base.quad_part };
}
VirtualAddressFormater GetPdteVirtualAddress(uint64_t vaddr)
{
    auto pte_base = LocatePteBase();
    if (pte_base.quad_part == 0) return { 0 };
    return { ((GetPteVirtualAddress(vaddr).quad_part >> 9) & VALID_VIRTUAL_ADDRESS_MASK) + pte_base.quad_part};
}
VirtualAddressFormater GetPpteVirtualAddress(uint64_t vaddr)
{
    auto pte_base = LocatePteBase();
    if (pte_base.quad_part == 0) return { 0 };
    return { ((GetPdteVirtualAddress(vaddr).quad_part >> 9) & VALID_VIRTUAL_ADDRESS_MASK) + pte_base.quad_part };
}
VirtualAddressFormater GetPxteVirtualAddress(uint64_t vaddr)
{
    auto pte_base = LocatePteBase();
    if (pte_base.quad_part == 0) return { 0 };
    return { ((GetPpteVirtualAddress(vaddr).quad_part >> 9) & VALID_VIRTUAL_ADDRESS_MASK) + pte_base.quad_part };
}
}



MmPte::Handware::Handware(MmPte* parent) : parent_(parent) {}
bool MmPte::Handware::Vaild() const {
    return parent_->pte_vaddr_.BitArea(0, 1);
}
uint64_t MmPte::Handware::PageFrameNumber() const {
    return parent_->pte_vaddr_.BitArea(PageFrameNumberBitPos, PageFrameNumberBitSize);
}
char MmPte::Handware::NoExecute() {
    return parent_->pte_vaddr_.BitArea(63, 1);
}
void MmPte::Handware::SetNoExecute(bool no_exec) {
    parent_->pte_vaddr_.WriteBitArea(63, no_exec ? 1 : 0, 1);
}
char MmPte::Handware::Write() {
    return parent_->pte_vaddr_.BitArea(WriteBitPos, WriteBitSize);
}
void MmPte::Handware::SetWrite(bool writable) {
    parent_->pte_vaddr_.WriteBitArea(WriteBitPos, writable ? 1 : 0, WriteBitSize);
}
char MmPte::Handware::CopyOnWrite() {
    return parent_->pte_vaddr_.BitArea(CopyOnWriteBitPos, CopyOnWriteBitSize);
}
void MmPte::Handware::SetCopyOnWrite(bool copy_on_writable) {
    parent_->pte_vaddr_.WriteBitArea(CopyOnWriteBitPos, copy_on_writable ? 1 : 0, CopyOnWriteBitSize);
}



void MmPte::Init()
{
    Handware::PageFrameNumberBitPos = 12;
    Handware::PageFrameNumberBitSize = 36;
    Handware::WriteBitPos = 11;
    Handware::WriteBitSize = 1;
    Handware::CopyOnWriteBitPos = 9;
    Handware::CopyOnWriteBitSize = 1;
}
MmPte::MmPte(MmVirtualAddress pte_vaddr): pte_vaddr_(pte_vaddr) {}
MmPte::Handware MmPte::Hand() {
    return { this };
}

void MmVirtualAddress::Init() {
    MmPte::Init();
}

MmVirtualAddress::MmVirtualAddress(uint64_t vaddr, PEPROCESS owner) noexcept
    : owner_(owner),
    vaddr_(vaddr) {}

MmVirtualAddress::MmVirtualAddress(void* ptr, PEPROCESS owner) noexcept
    : owner_(owner),
    vaddr_((uint64_t)ptr) {}

uint64_t MmVirtualAddress::PxtIndex() const noexcept {
    return VIRTUAL_ADDRESS_PXTI(vaddr_);
}
uint64_t MmVirtualAddress::PptIndex() const noexcept {
    return VIRTUAL_ADDRESS_PPTI(vaddr_);
}
uint64_t MmVirtualAddress::PdtIndex() const noexcept {
    return VIRTUAL_ADDRESS_PDTI(vaddr_);
}
uint64_t MmVirtualAddress::PtIndex() const noexcept {
    return VIRTUAL_ADDRESS_PTI(vaddr_);
}
uint64_t MmVirtualAddress::PteOffset() const noexcept {
    return VIRTUAL_ADDRESS_OFFSET(vaddr_);
}
MmPte MmVirtualAddress::GetPte() const {
    return MmVirtualAddress(GetPteUnsafe(), owner_);
}
PdteFormater* MmVirtualAddress::GetPdte() const {
    MmVirtualAddress ptr = GetPdteUnsafe();
    ptr.ThrowIfInvalid("解析出来的Pdte是无效地址: ");
    return ptr.Pointer<PdteFormater>();
}
PpteFormater* MmVirtualAddress::GetPpte() const {
    MmVirtualAddress ptr = GetPpteUnsafe();
    ptr.ThrowIfInvalid("解析出来的Ppte是无效地址: ");
    return ptr.Pointer<PpteFormater>();
}
PxteFormater* MmVirtualAddress::GetPxte() const {
    MmVirtualAddress ptr = GetPxteUnsafe();
    ptr.ThrowIfInvalid("解析出来的Pxte是无效地址: ");
    return ptr.Pointer<PxteFormater>();
}
bool MmVirtualAddress::IsValid() const {
    return MmIsAddressValid((void*)vaddr_);
}
uint8_t* MmVirtualAddress::PtrU8() const {
    return Pointer<uint8_t>();
}
uint16_t* MmVirtualAddress::PtrU16() const {
    return Pointer<uint16_t>();
}
uint32_t* MmVirtualAddress::PtrU32() const {
    return Pointer<uint32_t>();
}
uint64_t* MmVirtualAddress::PtrU64() const {
    return Pointer<uint64_t>();
}
uint64_t MmVirtualAddress::Address() const {
    return vaddr_;
}
std::vector<char> MmVirtualAddress::Buffer(size_t size) const
{
    ProcessAutoAttacker pa{ owner_ };
    std::vector<char> buf;
    buf.resize(size);
    RtlCopyMemory(buf.data(), Pointer(), size);
    return buf;
}
uint64_t MmVirtualAddress::ValU64() const {
    return Value<uint64_t>();
}
uint32_t MmVirtualAddress::ValU32() const {
    return Value<uint32_t>();
}
uint16_t MmVirtualAddress::ValU16() const {
    return Value<uint16_t>();
}
uint8_t MmVirtualAddress::ValU8() const {
    return Value<uint8_t>();
}
uint64_t MmVirtualAddress::BitArea(size_t bit_pos, uint8_t bit_size) const
{
    if (bit_size > 64) std::_Xinvalid_argument("bit_size必须小于64!");
    ThrowIfReadInvalid();

    uint64_t value = 0;
    uint8_t* bytes_buf = Pointer<uint8_t>();

    for (uint64_t i = 0; i < bit_size; ++i) {
        uint64_t byte_idx = (bit_pos + i) / 8;
        uint64_t bit_idx = (bit_pos + i) % 8;

        uint64_t bit_val = (bytes_buf[byte_idx] >> bit_idx) & 1;
        value |= (bit_val << i);
    }

    return value;
}
int MmVirtualAddress::Protection() const {
    ProcessAutoAttacker pa{ owner_ };
    int protection = 0;
    auto pxte = GetPxte();
    auto ppte = GetPpte();
    if (ppte->bits.large_page) {
        if (!(pxte->bits.execute_disable && ppte->bits_1g.execute_disable))
            protection |= kPageExecutable;
        if (!(pxte->bits.read_write && ppte->bits_1g.read_write))
            protection |= kPageWriteable;
        return protection;
    }
    bool pxte_ppte_executable = !(pxte->bits.execute_disable && ppte->bits.execute_disable);
    bool pxte_ppte_readwrite = pxte->bits.read_write && ppte->bits.read_write;
    auto pdte = GetPdte();
    if (pdte->bits.large_page) {
        if (!pdte->bits_2m.execute_disable && pxte_ppte_executable)
            protection |= kPageExecutable;
        if (!pdte->bits_2m.read_write && pxte_ppte_readwrite)
            protection |= kPageExecutable;
        return protection;
    }
    bool pxte_ppte_pdte_executable = pxte_ppte_executable && !pdte->bits.execute_disable;
    bool pxte_ppte_pdte_readwrite = pxte_ppte_readwrite && pdte->bits.read_write;
    auto pte = GetPte();
    if (pxte_ppte_pdte_executable && !pte.Hand().NoExecute())
        protection |= kPageExecutable;
    if (pxte_ppte_pdte_readwrite && pte.Hand().Write())
        protection |= kPageWriteable;
    if (pte.Hand().CopyOnWrite())
        protection |= kPageCopyOnWrite;
    return protection;
}
PEPROCESS MmVirtualAddress::Owner() const {
    return owner_;
}
void MmVirtualAddress::WriteBuffer(size_t pos, void* buffer, size_t buf_size) const {
    ThrowIfWriteInvalid();
    ProcessAutoAttacker pa{ owner_ };
    RtlMoveMemory(Pointer(), buffer, buf_size);
}
void MmVirtualAddress::WriteBitArea(size_t beg_bit_pos, uint64_t src_value, size_t src_bit_size) const {
    if (src_bit_size > 64) std::_Xinvalid_argument("src_bit_size必须小于64!");
    ThrowIfWriteInvalid();

    uint8_t* bytes_buf = Pointer<uint8_t>();

    for (size_t i = 0; i < src_bit_size; ++i) {
        size_t byte_idx = (beg_bit_pos + i) / 8;
        size_t bit_idx = (beg_bit_pos + i) % 8;

        uint64_t bit_val = (src_value >> i) & 1;

        bytes_buf[byte_idx] &= ~(1 << bit_idx);
        bytes_buf[byte_idx] |= (bit_val << bit_idx);
    }
}
bool MmVirtualAddress::SetProtection(int protection) const {
    ProcessAutoAttacker pa{ owner_ };
    bool is_executable = protection & kPageExecutable;
    bool is_writeable = protection & kPageWriteable;
    bool is_writecopy = protection & kPageCopyOnWrite;
    if (is_writeable && is_writecopy) return false;
    auto pxte = GetPxte();
    auto ppte = GetPpte();

    if (is_executable)
        pxte->bits.execute_disable = 0;
    if (is_writeable)
        pxte->bits.read_write = 1;

    if (ppte->bits.large_page) {
        ppte->bits_1g.execute_disable = !is_executable;
        ppte->bits_1g.read_write = is_writeable;
        return true;
    }

    auto pdte = GetPdte();

    if (is_executable)
        ppte->bits.execute_disable = 0;
    if (is_writeable)
        ppte->bits.read_write = 1;

    if (pdte->bits.large_page) {
        pdte->bits_2m.execute_disable = !is_executable;
        pdte->bits_2m.read_write = is_writeable;
        return true;
    }

    auto pte = GetPte();

    if (is_executable)
        pdte->bits.execute_disable = 0;
    if (is_writeable)
        pdte->bits.read_write = 1;

    pte.Hand().SetNoExecute(!is_executable);
    pte.Hand().SetWrite(is_writeable);
    pte.Hand().SetCopyOnWrite(is_writecopy);
    return true;
}
void MmVirtualAddress::SetOwner(PEPROCESS eproc) noexcept {
    owner_ = eproc;
}
MmVirtualAddress MmVirtualAddress::operator+(ptrdiff_t offset) const noexcept {
    auto tmp = *this;
    tmp.vaddr_ += offset;
    return tmp;
}
MmVirtualAddress MmVirtualAddress::operator-(ptrdiff_t offset) const noexcept {
    auto tmp = *this;
    tmp.vaddr_ -= offset;
    return tmp;
}
MmVirtualAddress& MmVirtualAddress::operator+=(ptrdiff_t offset) noexcept {
    vaddr_ += offset;
    return *this;
}
MmVirtualAddress& MmVirtualAddress::operator-=(ptrdiff_t offset) noexcept {
    vaddr_ -= offset;
    return *this;
}
void MmVirtualAddress::ThrowIfReadInvalid() const {
    ThrowIfInvalid("读取了无效内存:");
}
void MmVirtualAddress::ThrowIfWriteInvalid() const {
    ThrowIfInvalid("写入了无效内存:");
}
void MmVirtualAddress::ThrowIfInvalid(const char* format) const {
    if (!IsValid()) {
        auto err = fmt::format("{}{:016x}\n", format, Address());
        std::_Xruntime_error(err.c_str());
    }
}
bool operator==(MmVirtualAddress x, MmVirtualAddress y) noexcept {
    return x.vaddr_ == y.vaddr_;
}
bool operator!=(MmVirtualAddress x, MmVirtualAddress y) noexcept {
    return x.vaddr_ != y.vaddr_;
}
bool operator>(MmVirtualAddress x, MmVirtualAddress y) noexcept {
    return x.vaddr_ > y.vaddr_;
}
bool operator<(MmVirtualAddress x, MmVirtualAddress y) noexcept {
    return x.vaddr_ < y.vaddr_;
}
bool operator>=(MmVirtualAddress x, MmVirtualAddress y) noexcept {
    return x.vaddr_ >= y.vaddr_;
}
bool operator<=(MmVirtualAddress x, MmVirtualAddress y) noexcept {
    return x.vaddr_ <= y.vaddr_;
}
MmVirtualAddress operator+(ptrdiff_t offset, MmVirtualAddress next) noexcept {
    return next + offset;
}

void* MmVirtualAddress::GetPteUnsafe() const {
    return details::GetPteVirtualAddress(vaddr_).ptr;
}
void* MmVirtualAddress::GetPdteUnsafe() const {
    return details::GetPdteVirtualAddress(vaddr_).ptr;
}
void* MmVirtualAddress::GetPpteUnsafe() const {
    return details::GetPpteVirtualAddress(vaddr_).ptr;
}
void* MmVirtualAddress::GetPxteUnsafe() const {
    return details::GetPxteVirtualAddress(vaddr_).ptr;
}
}