#include "ethread.h"

namespace StarryEye {
void EThread::Init()
{
	ThreadListEntryOffset = 0x4e8;
}

EThread::EThread(uint64_t address) : KObjectBase(address) {}

KThread EThread::Tcb()
{
	return KThread(address_);
}

ListEntry EThread::ThreadListEntry()
{
	return ListEntry(address_ + ThreadListEntryOffset, ThreadListEntryOffset);
}
}