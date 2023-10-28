#include "object_type.h"

namespace stareye {
void ObjectType::Init()
{
	ObTypeIndexTable = MmVirtualAddress(0xfffff802598fce80);	//TODO ObTypeIndexTable
	NameOffset = 0x10;
}

PUNICODE_STRING ObjectType::Name()
{
	return (vaddr_ + NameOffset).Pointer<UNICODE_STRING>();
}

bool ObjectType::CompareTypeName(PCWSTR name)
{
	UNICODE_STRING str;
	RtlInitUnicodeString(&str, name);
	return RtlCompareUnicodeString(Name(), &str, TRUE) == 0;
}
}