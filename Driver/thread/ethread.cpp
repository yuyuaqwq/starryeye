#include "ethread.h"

namespace StarryEye {
void EThread::Init()
{
	ThreadListEntryOffset = 0x4e8;
}

EThread::EThread(uint64_t address) : KObjectBase(address) {}
EThread::EThread(std::nullptr_t) : KObjectBase(nullptr) {}
EThread::~EThread() {}

KThread EThread::Tcb()
{
	return KThread(address_);
}

ListEntry EThread::ThreadListEntry()
{
	return ListEntry(address_ + ThreadListEntryOffset, ThreadListEntryOffset);
}
}