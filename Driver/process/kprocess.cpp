#include "kprocess.h"

namespace StarryEye {
void KProcess::Init()
{
    ProcessListEntryOffset = 0x350;
    DirectoryTableBaseOffset = 0x28;
}

KProcess::KProcess(uint64_t address) : KObjectBase(address) {}

ListEntry KProcess::ProcessListEntry()
{
    return ListEntry(address_ + ProcessListEntryOffset, ProcessListEntryOffset);
}
uint64_t KProcess::DirectoryTableBase()
{
    return *(uint64_t*)(address_ + DirectoryTableBaseOffset);
}
}