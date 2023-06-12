#include "kthread.h"

using namespace StarryEye;

void StarryEye::KThread::Init()
{
	ProcessOffset = 0x220;
}

StarryEye::KThread::KThread(ULONG64 address): KObjectBase(address)
{
}

StarryEye::KThread::KThread(std::nullptr_t): KObjectBase(nullptr)
{
}

StarryEye::KThread::~KThread()
{
}

KProcess StarryEye::KThread::Process()
{
	return KProcess(*(PULONG64)(address_ + ProcessOffset));
}
