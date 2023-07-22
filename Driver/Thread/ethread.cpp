#include "ethread.h"

namespace StarryEye {
void EThread::Init()
{
	ThreadListEntryOffset = 0x4e8;
}

EThread::EThread(ULONG64 address) : KObjectBase(address) {}
EThread::EThread(std::nullptr_t) : KObjectBase(nullptr) {}
EThread::~EThread() {}

KThread EThread::Tcb()
{
	return KThread(address_);
}

KObjListEntry<EThread> EThread::ThreadListEntry()
{
	return KObjListEntry<EThread>(address_ + ThreadListEntryOffset, ThreadListEntryOffset);
}
}