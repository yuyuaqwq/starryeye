#include "kprocess.h"

namespace StarryEye {
void KProcess::Init()
{
    ProcessListEntryOffset = 0x350;
}

KProcess::KProcess(ULONG64 address) : KObjectBase(address) {}
KProcess::KProcess(std::nullptr_t) : KObjectBase(nullptr) {}
KProcess::~KProcess() {}

KObjListEntry<KProcess> KProcess::ProcessListEntry()
{
    return KObjListEntry<KProcess>(address_ + ProcessListEntryOffset, ProcessListEntryOffset);
}
}