#include "base.h"

StarryEye::KObjectBase::KObjectBase(ULONG64 address): address_(address) {}
StarryEye::KObjectBase::KObjectBase(std::nullptr_t): address_(0) {}
StarryEye::KObjectBase::~KObjectBase() { address_ = 0; }

bool StarryEye::KObjectBase::IsVaild()
{
	return MmIsAddressValid((PVOID)address_);
}

ULONG64 StarryEye::KObjectBase::Address()
{
	return address_;
}
