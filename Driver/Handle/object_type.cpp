#include "object_type.h"

StarryEye::ObjectType::ObjectType(ULONG64 address)
{
	address_ = address;
	name_offset_ = 0x10;		//TODO NameÆ«ÒÆ
}

StarryEye::ObjectType::ObjectType(std::nullptr_t)
{
	address_ = 0;
}

StarryEye::ObjectType::~ObjectType()
{
}

bool StarryEye::ObjectType::IsValid()
{
	return MmIsAddressValid((PVOID)address_);
}

PUNICODE_STRING StarryEye::ObjectType::Name()
{
	return (PUNICODE_STRING)(name_offset_ + address_);
}

bool StarryEye::ObjectType::IsProcess()
{
	return RtlCompareUnicodeString(Name(), &ProcessType, FALSE) == 0;
}

bool StarryEye::ObjectType::IsThread()
{
	return RtlCompareUnicodeString(Name(), &ThreadType, FALSE) == 0;
}
