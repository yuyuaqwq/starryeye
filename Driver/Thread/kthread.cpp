#include "kthread.h"

namespace StarryEye {
void KThread::Init()
{
	ProcessOffset = 0x220;
}

KThread::KThread(ULONG64 address) : KObjectBase(address) {}
KThread::KThread(std::nullptr_t) : KObjectBase(nullptr) {}
KThread::~KThread() {}

KProcess KThread::Process()
{
	return KProcess(*(PULONG64)(address_ + ProcessOffset));
}
}