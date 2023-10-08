#include "ethread.h"

namespace stareye {
void EThread::Init()
{
	ThreadListEntryOffset = 0x4e8;
}

KThread EThread::Tcb() {
	return vaddr_;
}

ListEntry<EThread> EThread::ThreadListEntry() {
	return ListEntry<EThread>(vaddr_ + ThreadListEntryOffset, ThreadListEntryOffset);
}
}