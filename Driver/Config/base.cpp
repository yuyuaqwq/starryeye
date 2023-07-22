#include "base.h"

namespace StarryEye {
KObjectBase::KObjectBase(ULONG64 address) : address_(address) {}
KObjectBase::KObjectBase(std::nullptr_t) : address_(0) {}
KObjectBase::~KObjectBase() { address_ = 0; }

bool KObjectBase::IsVaild()
{
	return MmIsAddressValid((PVOID)address_);
}

ULONG64 KObjectBase::Address()
{
	return address_;
}
}