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



bool MmPte::Handware::IsVaild() {
    return (*(uint64_t*)address_ & 0x1) == 1;
}
uint64_t MmPte::Handware::PageFrameNumber() {
    return GetBitAreaValue((uint64_t*)address_, 8, PageFrameNumberBitPos, PageFrameNumberBitSize).SomeVal();
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



MmVirtualAddress::MmVirtualAddress(uint64_t va, uint64_t owner_kproc_addr)
    : KObjectBase(va),
    owner_kproc_addr_(owner_kproc_addr){}

uint64_t MmVirtualAddress::PxtIndex() {
    return VIRTUAL_ADDRESS_PXTI(address_);
}
uint64_t MmVirtualAddress::PptIndex() {
    return VIRTUAL_ADDRESS_PPTI(address_);
}
uint64_t MmVirtualAddress::PdtIndex() {
    return VIRTUAL_ADDRESS_PDTI(address_);
}
uint64_t MmVirtualAddress::PtIndex() {
    return VIRTUAL_ADDRESS_PTI(address_);
}
uint64_t MmVirtualAddress::Offset() {
    return VIRTUAL_ADDRESS_OFFSET(address_);
}
MmPte MmVirtualAddress::GetPte() {
    return MmPte(details::GetPteVirtualAddress(address_).quad_part);
}
PdteFormater* MmVirtualAddress::GetPdte() {
    return reinterpret_cast<PdteFormater*>(details::GetPdteVirtualAddress(address_).ptr);
}
PpteFormater* MmVirtualAddress::GetPpte() {
    return reinterpret_cast<PpteFormater*>(details::GetPpteVirtualAddress(address_).ptr);
}
PxteFormater* MmVirtualAddress::GetPxte() {
    return reinterpret_cast<PxteFormater*>(details::GetPxteVirtualAddress(address_).ptr);
}
}