#include "object_type.h"

StarryEye::ObjectType::ObjectType(ULONG64 address) : KObjectBase(address)
{
	name_offset_ = 0x10;		//TODO NameÆ«ÒÆ
}

StarryEye::ObjectType::ObjectType(std::nullptr_t) : KObjectBase(nullptr)
{
	name_offset_ = 0x10;
}

StarryEye::ObjectType::~ObjectType()
{
}

PUNICODE_STRING StarryEye::ObjectType::Name()
{
	return (PUNICODE_STRING)(name_offset_ + address_);
}

bool StarryEye::ObjectType::IsProcess()
{
	return RtlCompareUnicodeString(Name(), &ProcessTypeString, TRUE) == 0;
}

bool StarryEye::ObjectType::IsThread()
{
	return RtlCompareUnicodeString(Name(), &ThreadTypeString, TRUE) == 0;
}
