#include "kthread.h"

namespace stareye {
void KThread::Init()
{
	ProcessOffset = 0x220;
}

KThread::KThread(const MmVirtualAddress& vaddr) : KObject(vaddr) {}

KProcess KThread::Process() {
	return (vaddr_ + ProcessOffset).ValU64();
}
}