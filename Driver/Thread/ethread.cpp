#include "ethread.h"

StarryEye::EThread::EThread(ULONG64 address) : KObjectBase(address)
{
}

StarryEye::EThread::EThread(std::nullptr_t) : KObjectBase(nullptr)
{
}

StarryEye::EThread::~EThread()
{
}
