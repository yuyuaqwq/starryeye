#include "object_type.h"

namespace StarryEye {
void ObjectType::Init()
{
	ObTypeIndexTable = (uint64_t*)0xfffff80260efce80;	//TODO ObTypeIndexTable
	NameOffset = 0x10;
}

ObjectType::ObjectType(uint64_t address) : KObjectBase(address) {}
ObjectType::ObjectType(std::nullptr_t) : KObjectBase(nullptr) {}
ObjectType::~ObjectType() {}

PUNICODE_STRING ObjectType::Name()
{
	return (PUNICODE_STRING)(NameOffset + address_);
}

bool ObjectType::CompareTypeName(PCWSTR name)
{
	UNICODE_STRING str;
	RtlInitUnicodeString(&str, name);
	return RtlCompareUnicodeString(Name(), &str, TRUE) == 0;
}
}