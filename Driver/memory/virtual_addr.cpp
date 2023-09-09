#include "memory/virtual_addr.h"
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

VirtualAddressFormater GetPteVirtualAddress(uint64_t address)
{
    auto pte_base = LocatePteBase();
    if (pte_base.quad_part == 0) return { 0 };
    return { ((address >> 9) & VALID_VIRTUAL_ADDRESS_MASK) + pte_base.quad_part };
}
VirtualAddressFormater GetPdteVirtualAddress(uint64_t address)
{
    auto pte_base = LocatePteBase();
    if (pte_base.quad_part == 0) return { 0 };
    return { ((GetPteVirtualAddress(address).quad_part >> 9) & VALID_VIRTUAL_ADDRESS_MASK) + pte_base.quad_part};
}
VirtualAddressFormater GetPpteVirtualAddress(uint64_t address)
{
    auto pte_base = LocatePteBase();
    if (pte_base.quad_part == 0) return { 0 };
    return { ((GetPdteVirtualAddress(address).quad_part >> 9) & VALID_VIRTUAL_ADDRESS_MASK) + pte_base.quad_part };
}
VirtualAddressFormater GetPxteVirtualAddress(uint64_t address)
{
    auto pte_base = LocatePteBase();
    if (pte_base.quad_part == 0) return { 0 };
    return { ((GetPpteVirtualAddress(address).quad_part >> 9) & VALID_VIRTUAL_ADDRESS_MASK) + pte_base.quad_part };
}
}



bool MmPte::Handware::Vaild() const {
    return (*pte_ptr_ & 0x1) == 1;
}
uint64_t MmPte::Handware::PageFrameNumber() {
    return GetBitAreaValue(pte_ptr_, 8, PageFrameNumberBitPos, PageFrameNumberBitSize).Default(~0ull);
}
char MmPte::Handware::NoExecute()
{
    return (*pte_ptr_) >> 63;
}
void MmPte::Handware::SetNoExecute(bool no_exec)
{
    if (no_exec) {
        (*pte_ptr_) |= (1ull << 63);
    }
    else {
        (*pte_ptr_) &= (~(1ull << 63));
    }
}
char MmPte::Handware::Write() {
    return GetBitAreaValue(pte_ptr_, 8, WriteBitPos, WriteBitSize).Default(0);
}
void MmPte::Handware::SetWrite(bool writable) {
    SetBitAreaValue(pte_ptr_, 8, WriteBitPos, writable ? 1 : 0, WriteBitSize);
}
char MmPte::Handware::CopyOnWrite() {
    return GetBitAreaValue(pte_ptr_, 8, CopyOnWriteBitPos, CopyOnWriteBitSize).Default(0);
}
void MmPte::Handware::SetCopyOnWrite(bool copy_on_writable) {
    SetBitAreaValue(pte_ptr_, 8, CopyOnWriteBitPos, copy_on_writable ? 1 : 0, CopyOnWriteBitSize);
}

MmPte::Handware::Handware(void* pte_ptr) : pte_ptr_(static_cast<uint64_t*>(pte_ptr)) {}
void MmPte::Init()
{
    Handware::PageFrameNumberBitPos = 12;
    Handware::PageFrameNumberBitSize = 36;
    Handware::WriteBitPos = 11;
    Handware::WriteBitSize = 1;
    Handware::CopyOnWriteBitPos = 9;
    Handware::CopyOnWriteBitSize = 1;
}
MmPte::MmPte(void* pte_ptr) : pte_ptr_(static_cast<uint64_t*>(pte_ptr)) {}
MmPte::Handware MmPte::Hand() {
    return { pte_ptr_ };
}

void MmVirtualAddress::Init()
{
    MmPte::Init();
}
MmVirtualAddress::MmVirtualAddress()
    : owner_(nullptr),
    mem_size_(0) {}

MmVirtualAddress::MmVirtualAddress(uint64_t vaddr, size_t mem_size, PEPROCESS owner)
    : owner_(owner),
    mem_size_(mem_size),
    vaddr_(vaddr) {}

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
uint64_t MmVirtualAddress::Offset() const {
    return VIRTUAL_ADDRESS_OFFSET(vaddr_);
}
MmPte MmVirtualAddress::GetPte() const {
    return MmPte(details::GetPteVirtualAddress(vaddr_).ptr);
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
std::vector<char> MmVirtualAddress::ReadBuffer(size_t size, size_t pos) const
{
    ProcessAutoAttacker pa{ owner_ };
    std::vector<char> buf;
    buf.resize(size);
    RtlCopyMemory(buf.data(), Pointer(pos), size);
    return buf;
}
uint64_t MmVirtualAddress::ReadUint64(size_t pos) const
{
    return ReadValue<uint64_t>();
}
bool MmVirtualAddress::WriteBuffer(size_t pos, void* buffer, size_t buf_size)
{
    if (buf_size > (pos + mem_size_)) return false;
    ProcessAutoAttacker pa{ owner_ };
    RtlMoveMemory(Pointer(pos), buffer, buf_size);
}
bool operator==(const MmVirtualAddress& x, const MmVirtualAddress& y) {
    return x.vaddr_ == y.vaddr_ &&
        x.owner_ == y.owner_ &&
        x.mem_size_ == y.mem_size_;
}
bool operator!=(const MmVirtualAddress& x, const MmVirtualAddress& y) {
    return !(x == y);
}
}