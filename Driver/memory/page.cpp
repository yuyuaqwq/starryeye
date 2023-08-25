#include "memory/page.h"
#include "process/kprocess.h"

namespace StarryEye {
namespace details {
void MmPageHandware::Init()
{
    LargePageBitPos = 7;
    LargePageBitSize = 1;
    PageFrameNumberBitPos = 12;
    PageFrameNumberBitSize = 36;
}
MmPageHandware::MmPageHandware(uint64_t pte): pte_(pte) {}
uint64_t MmPageHandware::Valid() {
    return pte_ & 0x1;
}
uint64_t MmPageHandware::LargePage() {
    return GetBitAreaValue(&pte_, sizeof(pte_), LargePageBitPos, LargePageBitSize).SomeVal();
}
uint64_t MmPageHandware::PageFrameNumber() {
    return GetBitAreaValue(&pte_, sizeof(pte_), PageFrameNumberBitPos, PageFrameNumberBitSize).SomeVal();
}
}

MmPage::MmPage(std::nullptr_t)
    : KObjectBase(nullptr),
    pte_(0) {}

MmPage::MmPage(uint64_t pte)
    : KObjectBase(nullptr),
    pte_(pte) {}
details::MmPageHandware MmPage::Hand() {
    return pte_;
}


MmPageTable::MmPageTable(std::nullptr_t)
    : KObjectBase(nullptr),
    ulonglong_(0) {}
MmPageTable::MmPageTable(uint64_t pt)
    : KObjectBase(nullptr),
    ulonglong_(pt) {}
uint64_t MmPageTable::Address() {
    return Is2mPage() ? pdte_2m_.page_align << 21 : pdte_.pdt_align << 12;
}
bool MmPageTable::Is2mPage() {
    return pdte_.large_page;
}
MmPage MmPageTable::operator[](size_t idx) {
    return ((uint64_t*)Address())[idx];
}


MmPageDirectoryTable::MmPageDirectoryTable(std::nullptr_t)
    : KObjectBase(nullptr),
    ulonglong_(0) {}
MmPageDirectoryTable::MmPageDirectoryTable(uint64_t pdt)
    : KObjectBase(nullptr),
    ulonglong_(pdt) {}
bool MmPageDirectoryTable::Is1gPage() {
    return pdpte_.large_page;
}
uint64_t MmPageDirectoryTable::Address() {
    return Is1gPage() ? pdpte_1g_.page_align << 30 : pdpte_.pdpt_align << 12;
}
MmPageTable MmPageDirectoryTable::operator[](size_t idx) {
    return ((uint64_t*)Address())[idx];
}


MmPageDirectoryPointerTable::MmPageDirectoryPointerTable(std::nullptr_t)
    : KObjectBase(nullptr),
    ulonglong_(0) {}
MmPageDirectoryPointerTable::MmPageDirectoryPointerTable(uint64_t pml4e)
    : KObjectBase(nullptr),
    ulonglong_(pml4e) {}
uint64_t MmPageDirectoryPointerTable::Address() {
    return pml4e_.pdpt_align << 12;
}
MmPageDirectoryTable MmPageDirectoryPointerTable::operator[](size_t idx) {
    return ((uint64_t*)Address())[idx];
}


MmPageMapLevel4::MmPageMapLevel4(std::nullptr_t) : KObjectBase(nullptr), ulonglong_(0) {}
MmPageMapLevel4::MmPageMapLevel4(uint64_t cr3) : KObjectBase(nullptr), ulonglong_(cr3) {}
uint64_t MmPageMapLevel4::Address() {
    return cr3_.pml4_align << 12;
}
MmPageDirectoryPointerTable MmPageMapLevel4::operator[](size_t idx) {
    return ((uint64_t*)Address())[idx];
}

MmVirtualAddress::MmVirtualAddress(std::nullptr_t)
    : KObjectBase(nullptr),
    owner_kproc_addr_(0),
    vaddr_(0),
    directory_table_(nullptr){}
MmVirtualAddress::MmVirtualAddress(uint64_t va, uint64_t owner_kproc_addr)
    : KObjectBase(va),
    owner_kproc_addr_(owner_kproc_addr),
    vaddr_(va),
    directory_table_(KProcess(owner_kproc_addr).DirectoryTableBase()) {}

uint64_t MmVirtualAddress::Pml4Index() {
    return vaddr_bits.pml4_idx;
}
uint64_t MmVirtualAddress::PdptIndex() {
    return vaddr_bits.pdpt_idx;
}
uint64_t MmVirtualAddress::PdtIndex() {
    return vaddr_bits.pdt_idx;
}
uint64_t MmVirtualAddress::PtIndex() {
    return vaddr_bits.pt_idx;
}
uint64_t MmVirtualAddress::Offset() {
    return vaddr_bits.offset;
}
uint64_t MmVirtualAddress::ToPhysicalAddress()
{
    auto pte = directory_table_[Pml4Index()][PdptIndex()][PdtIndex()][PtIndex()];
    return (pte.Hand().PageFrameNumber() << 12) + Offset();
}
}