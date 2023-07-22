#include "object_type.h"

namespace StarryEye {
void ObjectType::Init()
{
	ObTypeIndexTable = (PULONG64)0xfffff80260efce80;	//TODO ObTypeIndexTable
	NameOffset = 0x10;
	RtlInitUnicodeString(&ProcessTypeString, L"Process");
	RtlInitUnicodeString(&ThreadTypeString, L"Thread");
}

ObjectType::ObjectType(ULONG64 address) : KObjectBase(address) {}
ObjectType::ObjectType(std::nullptr_t) : KObjectBase(nullptr) {}
ObjectType::~ObjectType() {}

PUNICODE_STRING ObjectType::Name()
{
	return (PUNICODE_STRING)(NameOffset + address_);
}

bool ObjectType::IsProcess()
{
	return RtlCompareUnicodeString(Name(), &ProcessTypeString, TRUE) == 0;
}

bool ObjectType::IsThread()
{
	return RtlCompareUnicodeString(Name(), &ThreadTypeString, TRUE) == 0;
}
}