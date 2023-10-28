#include "kthread.h"

namespace stareye {
void KThread::Init()
{
	ProcessOffset = 0x220;
}

KProcess KThread::Process() {
	return (vaddr_ + ProcessOffset).DerefAsAddr();
}
}