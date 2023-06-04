#include "kprocess.h"

StarryEye::KProcess::KProcess(ULONG64 address): KObjectBase(address)
{
}

StarryEye::KProcess::KProcess(std::nullptr_t): KObjectBase(nullptr)
{
}

StarryEye::KProcess::~KProcess()
{
}
