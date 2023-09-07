#include "base.h"

namespace StarryEye {
KObjectBase::KObjectBase(uint64_t address) : address_(address) {}
KObjectBase::KObjectBase() : address_(0) {}
KObjectBase::~KObjectBase() { address_ = 0; }

bool KObjectBase::IsVaild() const
{
	return MmIsAddressValid((PVOID)Address());
}

uint64_t KObjectBase::Address() const
{
	return address_;
}
}