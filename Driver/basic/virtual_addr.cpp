#include "virtual_addr.h"
#include "process/kprocess.h"

namespace StarryEye {
namespace details {
VirtualAddressFormater LocatePxtBase()
{
    inline static VirtualAddressFormater PxtBaseCache{0};
    if (PxtBaseCache.quad_part == 0)
    {
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
    inline static VirtualAddressFormater PteBaseCache{0};
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



bool MmPte::Handware::Vaild() const {
    return parent_->pte_vaddr_.BitArea(0, 1);
}
uint64_t MmPte::Handware::PageFrameNumber() {
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
MmPte::MmPte(const MmVirtualAddress& pte_vaddr): pte_vaddr_(pte_vaddr) {}
MmPte::Handware MmPte::Hand() {
    return { this };
}

void MmVirtualAddress::Init() {
    MmPte::Init();
}

MmVirtualAddress::MmVirtualAddress(uint64_t vaddr, PEPROCESS owner)
    : owner_(owner),
    vaddr_(vaddr) {}

MmVirtualAddress::MmVirtualAddress(void* ptr, PEPROCESS owner)
    : owner_(owner),
    vaddr_((uint64_t)ptr) {}

uint64_t MmVirtualAddress::PxtIndex() const {
    return VIRTUAL_ADDRESS_PXTI(vaddr_);
}
uint64_t MmVirtualAddress::PptIndex() const {
    return VIRTUAL_ADDRESS_PPTI(vaddr_);
}
uint64_t MmVirtualAddress::PdtIndex() const {
    return VIRTUAL_ADDRESS_PDTI(vaddr_);
}
uint64_t MmVirtualAddress::PtIndex() const {
    return VIRTUAL_ADDRESS_PTI(vaddr_);
}
uint64_t MmVirtualAddress::PteOffset() const {
    return VIRTUAL_ADDRESS_OFFSET(vaddr_);
}
MmPte MmVirtualAddress::GetPte() const {
    return MmPte({ details::GetPteVirtualAddress(vaddr_).quad_part, owner_ });
}
PdteFormater* MmVirtualAddress::GetPdte() const {
    return reinterpret_cast<PdteFormater*>(details::GetPdteVirtualAddress(vaddr_).ptr);
}
PpteFormater* MmVirtualAddress::GetPpte() const {
    return reinterpret_cast<PpteFormater*>(details::GetPpteVirtualAddress(vaddr_).ptr);
}
PxteFormater* MmVirtualAddress::GetPxte() const {
    return reinterpret_cast<PxteFormater*>(details::GetPxteVirtualAddress(vaddr_).ptr);
}
bool MmVirtualAddress::IsValid() const {
    return MmIsAddressValid(Pointer());
}
uint64_t MmVirtualAddress::Address() const
{
    return vaddr_;
}
krnlib::vector<char> MmVirtualAddress::Buffer(size_t size) const
{
    ProcessAutoAttacker pa{ owner_ };
    krnlib::vector<char> buf;
    buf.resize(size);
    RtlCopyMemory(buf.data(), Pointer(), size);
    return buf;
}
uint64_t MmVirtualAddress::ValU64() {
    return Value<uint64_t>();
}
uint32_t MmVirtualAddress::ValU32() {
    return Value<uint32_t>();
}
uint16_t MmVirtualAddress::ValU16() {
    return Value<uint16_t>();
}
uint8_t MmVirtualAddress::ValU8() {
    return Value<uint8_t>();
}
uint64_t MmVirtualAddress::BitArea(size_t bit_pos, uint8_t bit_size)
{
    NT_ASSERT(bit_size > 64);

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
void MmVirtualAddress::WriteBuffer(size_t pos, void* buffer, size_t buf_size) const {
    ProcessAutoAttacker pa{ owner_ };
    RtlMoveMemory(Pointer(), buffer, buf_size);
}
void MmVirtualAddress::WriteBitArea(size_t beg_bit_pos, uint64_t src_value, size_t src_bit_size) const {
    NT_ASSERT(src_bit_size > 64);

    uint8_t* bytes_buf = Pointer<uint8_t>();

    for (size_t i = 0; i < src_bit_size; ++i) {
        size_t byte_idx = (beg_bit_pos + i) / 8;
        size_t bit_idx = (beg_bit_pos + i) % 8;

        uint64_t bit_val = (src_value >> i) & 1;

        bytes_buf[byte_idx] &= ~(1 << bit_idx);
        bytes_buf[byte_idx] |= (bit_val << bit_idx);
    }
}
MmVirtualAddress MmVirtualAddress::operator+(ptrdiff_t offset) {
    auto tmp = *this;
    tmp.vaddr_ += offset;
    return tmp;
}
MmVirtualAddress MmVirtualAddress::operator-(ptrdiff_t offset) {
    auto tmp = *this;
    tmp.vaddr_ -= offset;
    return tmp;
}
MmVirtualAddress& MmVirtualAddress::operator+=(ptrdiff_t offset) {
    vaddr_ += offset;
    return *this;
}
MmVirtualAddress& MmVirtualAddress::operator-=(ptrdiff_t offset) {
    vaddr_ -= offset;
    return *this;
}
bool operator==(const MmVirtualAddress& x, const MmVirtualAddress& y) {
    return x.vaddr_ == y.vaddr_ &&
        x.owner_ == y.owner_;
}
bool operator!=(const MmVirtualAddress& x, const MmVirtualAddress& y) {
    return !(x == y);
}
MmVirtualAddress operator+(ptrdiff_t offset, MmVirtualAddress next)
{
    return next += offset;
}
}