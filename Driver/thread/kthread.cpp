#include "kthread.h"

namespace stareye {
void KThread::Init()
{
	ProcessOffset = 0x220;
}

KThread::KThread(MmVirtualAddress vaddr) : KObject(vaddr) {}

KProcess KThread::Process() {
	return KProcess((vaddr_ + ProcessOffset).ValU64());
}
}