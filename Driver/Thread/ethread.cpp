#include "ethread.h"

using namespace StarryEye;

void StarryEye::EThread::Init()
{
	ThreadListEntryOffset = 0x4e8;
}

ULONG64 StarryEye::EThread::GetThreadListEntryOffset()
{
	return ThreadListEntryOffset;
}

StarryEye::EThread::EThread(ULONG64 address) : KObjectBase(address)
{
}

StarryEye::EThread::EThread(std::nullptr_t) : KObjectBase(nullptr)
{
}

StarryEye::EThread::~EThread()
{
}

KThread StarryEye::EThread::Tcb()
{
	return KThread(address_);
}

KObjListEntry<EThread> StarryEye::EThread::ThreadListEntry()
{
	return KObjListEntry<EThread>(address_ + ThreadListEntryOffset, ThreadListEntryOffset);
}
