#include "kthread.h"

StarryEye::KThread::KThread(ULONG64 address): KObjectBase(address)
{
}

StarryEye::KThread::KThread(std::nullptr_t): KObjectBase(nullptr)
{
}

StarryEye::KThread::~KThread()
{
}
