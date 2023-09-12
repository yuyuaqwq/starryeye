#include "kprocess.h"

namespace StarryEye {
void KProcess::Init()
{
    ProcessListEntryOffset = 0x350;
    DirectoryTableBaseOffset = 0x28;
}

KProcess::KProcess(const MmVirtualAddress& vaddr) : KObject(vaddr) {}

ListEntry<KProcess> KProcess::ProcessListEntry()
{
    return { vaddr_ + ProcessListEntryOffset, ProcessListEntryOffset };
}
uint64_t KProcess::DirectoryTableBase()
{
    return (vaddr_ + DirectoryTableBaseOffset).ValU64();
}
}