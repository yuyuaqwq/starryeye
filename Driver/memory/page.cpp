#include "memory/page.h"
#include "process/kprocess.h"

namespace StarryEye {
namespace details {
VirtualAddressFormater LocatePml4Base()
{
    for (size_t i = 0; i < PML4_SIZE / 8; i++)
    {
        details::VirtualAddressFormater vaddr{0};
        vaddr.bits.pxti = i;
        vaddr.bits.ppti = i;
        vaddr.bits.pdti = i;
        vaddr.bits.pti = i;
        auto phyaddr = MmGetPhysicalAddress((PVOID)vaddr.quad_part);
        if (__readcr3() == phyaddr.QuadPart) {
            return vaddr;
        }
    }
    return { MAXULONGLONG };
}
}



bool MmPte::Handware::IsVaild() {
    return (pte_ & 0x1) == 1;
}
uint64_t MmPte::Handware::PageFrameNumber() {
    return GetBitAreaValue(&pte_, 8, PageFrameNumberBitPos, PageFrameNumberBitSize).SomeVal();
}
MmPte::Handware::Handware(details::VirtualAddressFormater pte_base)
{
    pte_base_ = pte_base;
    address_ = pte_base_.quad_part;
    pte_ = *pte_base_.ptr;
}
void MmPte::Init()
{
    Handware::PageFrameNumberBitPos = 12;
    Handware::PageFrameNumberBitSize = 36;
}
MmPte::MmPte(details::VirtualAddressFormater pte_base)
{
    pte_base_ = pte_base;
    address_ = pte_base.quad_part;
}
MmPte::Handware MmPte::Hand() {
    return { pte_base_ };
}



MmPt::MmPt(details::VirtualAddressFormater pt_base)
{
    pt_base_ = pt_base;
    address_ = pt_base.quad_part;
}
MmPte MmPt::operator[](size_t pti)
{
    auto pxti = pt_base_.bits.pdti;
    auto ppti = pt_base_.bits.pti;
    auto pdti = pt_base_.bits.offset;
    auto page_base = pt_base_;
    page_base.bits.ppti = pxti;
    page_base.bits.pdti = ppti;
    page_base.bits.pti = pdti;
    page_base.bits.offset = pti * 8;
    return { page_base };
}



MmPdt::MmPdt(details::VirtualAddressFormater pdt_base)
{
    pdt_base_ = pdt_base;
    address_ = pdt_base.quad_part;
}
MmPt MmPdt::operator[](size_t pdti)
{
    auto pxti = pdt_base_.bits.pti;
    auto ppti = pdt_base_.bits.offset;
    auto pt_base = pdt_base_;
    pt_base.bits.pdti = pxti;
    pt_base.bits.pti = ppti;
    pt_base.bits.offset = pdti * 8;
    return { pt_base };
}



MmPpt::MmPpt(details::VirtualAddressFormater pdpt_base)
{
    ppt_base_ = pdpt_base;
    address_ = ppt_base_.quad_part;
}
MmPdt MmPpt::operator[](size_t ppti)
{
    auto pxti = ppt_base_.bits.offset;
    auto pdt_base = ppt_base_;
    pdt_base.bits.pti = pxti;
    pdt_base.bits.offset = ppti * 8;
    return { pdt_base };
}



MmPxt::MmPxt(uint64_t kproc_addr) {
    KeAttachProcess((PRKPROCESS)kproc_addr);
    pxt_base_ = details::LocatePml4Base();
    KeDetachProcess();
    address_ = pxt_base_.quad_part;
}
MmPpt MmPxt::operator[](size_t pxti)
{
    auto ppt_base = pxt_base_;
    ppt_base.bits.offset = pxti * 8;
    return { ppt_base };
}



MmVirtualAddress::MmVirtualAddress(uint64_t va, uint64_t owner_kproc_addr)
    : KObjectBase(va),
    owner_kproc_addr_(owner_kproc_addr),
    vaddr_({ va }),
    directory_table_(KProcess(owner_kproc_addr).DirectoryTableBase()) {}

//MmVirtualAddress MmVirtualAddress::FromPhysicalAddress(uint64_t phyaddr, uint64_t kproc_addr)
//{
//    KeAttachProcess((PRKPROCESS)kproc_addr);
//    auto pml4_vaddr = details::LocatePml4Base();
//    KeDetachProcess();
//
//}

uint64_t MmVirtualAddress::PxtIndex() {
    return vaddr_.bits.pxti;
}
uint64_t MmVirtualAddress::PptIndex() {
    return vaddr_.bits.ppti;
}
uint64_t MmVirtualAddress::PdtIndex() {
    return vaddr_.bits.pdti;
}
uint64_t MmVirtualAddress::PtIndex() {
    return vaddr_.bits.pti;
}
uint64_t MmVirtualAddress::Offset() {
    return vaddr_.bits.offset;
}
MmPte MmVirtualAddress::PageProperty()
{
    return directory_table_[PxtIndex()][PptIndex()][PdtIndex()][PtIndex()];
}
}