#include "kthread.h"

namespace StarryEye {
void KThread::Init()
{
	ProcessOffset = 0x220;
}

KThread::KThread(uint64_t address) : KObjectBase(address) {}

KProcess KThread::Process()
{
	return KProcess(*(uint64_t*)(address_ + ProcessOffset));
}
}