#include "object_type.h"

namespace StarryEye {
void ObjectType::Init()
{
	ObTypeIndexTable = (uint64_t*)0xfffff80260efce80;	//TODO ObTypeIndexTable
	NameOffset = 0x10;
}

ObjectType::ObjectType(const MmVirtualAddress& vaddr) : KObject(vaddr) {}

PUNICODE_STRING ObjectType::Name()
{
	return &(vaddr_ + NameOffset).Value<UNICODE_STRING>();
}

bool ObjectType::CompareTypeName(PCWSTR name)
{
	UNICODE_STRING str;
	RtlInitUnicodeString(&str, name);
	return RtlCompareUnicodeString(Name(), &str, TRUE) == 0;
}
}