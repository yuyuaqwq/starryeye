#include "ethread.h"

namespace StarryEye {
void EThread::Init()
{
	ThreadListEntryOffset = 0x4e8;
}

EThread::EThread(const MmVirtualAddress& vaddr) : KObject(vaddr) {}

KThread EThread::Tcb() {
	return vaddr_;
}

ListEntry<EThread> EThread::ThreadListEntry() {
	return ListEntry<EThread>(vaddr_ + ThreadListEntryOffset, ThreadListEntryOffset);
}
}