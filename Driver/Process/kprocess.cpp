#include "kprocess.h"

using namespace StarryEye;

void StarryEye::KProcess::Init()
{
    ProcessListEntryOffset = 0x350;
}

StarryEye::KProcess::KProcess(ULONG64 address): KObjectBase(address)
{
}

StarryEye::KProcess::KProcess(std::nullptr_t): KObjectBase(nullptr)
{
}

StarryEye::KProcess::~KProcess()
{
}

KObjListEntry<KProcess> StarryEye::KProcess::ProcessListEntry()
{
    return KObjListEntry<KProcess>(address_ + ProcessListEntryOffset, ProcessListEntryOffset);
}
