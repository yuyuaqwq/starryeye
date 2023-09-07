#include "memory/page.h"
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



bool MmPte::Handware::IsVaild() const {
    return (*(uint64_t*)address_ & 0x1) == 1;
}
uint64_t MmPte::Handware::PageFrameNumber() {
    return GetBitAreaValue((uint64_t*)address_, 8, PageFrameNumberBitPos, PageFrameNumberBitSize).SomeVal();
}
char MmPte::Handware::ExecuteDisable()
{
    return (*(uint64_t*)address_) >> 63;
}

void MmPte::Handware::SetExecuteDisable(bool disable)
{
    if (disable) {
        (*(uint64_t*)address_) |= (1ull << 63);
    }
    else {
        (*(uint64_t*)address_) &= (~(1ull << 63));
    }
}

MmPte::Handware::Handware(uint64_t address): KObjectBase(address){}
void MmPte::Init()
{
    Handware::PageFrameNumberBitPos = 12;
    Handware::PageFrameNumberBitSize = 36;
}
MmPte::MmPte(uint64_t address): KObjectBase(address) {}
MmPte::Handware MmPte::Hand() {
    return { address_ };
}

MmVirtualAddress::MmVirtualAddress()
    : owner_kproc_addr_(0),
    max_size_(0) {}

MmVirtualAddress::MmVirtualAddress(uint64_t va, uint64_t owner_kproc_addr, size_t size)
    : KObjectBase(va),
    owner_kproc_addr_(owner_kproc_addr),
    max_size_(size){}

uint64_t MmVirtualAddress::PxtIndex() const {
    return VIRTUAL_ADDRESS_PXTI(address_);
}
uint64_t MmVirtualAddress::PptIndex() const {
    return VIRTUAL_ADDRESS_PPTI(address_);
}
uint64_t MmVirtualAddress::PdtIndex() const {
    return VIRTUAL_ADDRESS_PDTI(address_);
}
uint64_t MmVirtualAddress::PtIndex() const {
    return VIRTUAL_ADDRESS_PTI(address_);
}
uint64_t MmVirtualAddress::Offset() const {
    return VIRTUAL_ADDRESS_OFFSET(address_);
}
MmPte MmVirtualAddress::GetPte() const {
    return MmPte(details::GetPteVirtualAddress(address_).quad_part);
}
PdteFormater* MmVirtualAddress::GetPdte() const {
    return reinterpret_cast<PdteFormater*>(details::GetPdteVirtualAddress(address_).ptr);
}
PpteFormater* MmVirtualAddress::GetPpte() const {
    return reinterpret_cast<PpteFormater*>(details::GetPpteVirtualAddress(address_).ptr);
}
PxteFormater* MmVirtualAddress::GetPxte() const {
    return reinterpret_cast<PxteFormater*>(details::GetPxteVirtualAddress(address_).ptr);
}
bool MmVirtualAddress::WriteMemory(void* buf, size_t size) const
{
    if (max_size_ != 0 && size <= max_size_)
    {
        RtlCopyMemory((void*)address_, buf, size);
        return true;
    }
    return false;
}
}