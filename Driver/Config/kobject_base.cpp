#include "kobject_base.h"

StarryEye::KObjectBase::KObjectBase(ULONG64 address)
{
	address_ = address;
}

StarryEye::KObjectBase::KObjectBase(std::nullptr_t)
{
	address_ = 0;
}

StarryEye::KObjectBase::~KObjectBase()
{
}

bool StarryEye::KObjectBase::IsVaild()
{
	return MmIsAddressValid((PVOID)address_);
}
