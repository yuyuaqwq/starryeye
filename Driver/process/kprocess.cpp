#include "kprocess.h"

namespace stareye {
void KProcess::Init()
{
    ProcessListEntryOffset = 0x350;
    DirectoryTableBaseOffset = 0x28;
}

KProcess::KProcess(const MmVirtualAddress& vaddr) : KObject(vaddr) {
    vaddr_.SetOwner((PEPROCESS)vaddr.Address());
}

ListEntry<KProcess> KProcess::ProcessListEntry()
{
    return ListEntry<KProcess>(vaddr_ + ProcessListEntryOffset, ProcessListEntryOffset);
}
uint64_t KProcess::DirectoryTableBase()
{
    return (vaddr_ + DirectoryTableBaseOffset).ValU64();
}
}