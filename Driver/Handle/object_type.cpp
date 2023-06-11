#include "object_type.h"

using namespace StarryEye;

void StarryEye::ObjectType::Init()
{
	ObTypeIndexTable = (PULONG64)0xfffff8066f911e80;
	NameOffset = 0x10;
	RtlInitUnicodeString(&ProcessTypeString, L"Process");
	RtlInitUnicodeString(&ThreadTypeString, L"Thread");
}

StarryEye::ObjectType::ObjectType(ULONG64 address) : KObjectBase(address)
{
}

StarryEye::ObjectType::ObjectType(std::nullptr_t) : KObjectBase(nullptr)
{
}

StarryEye::ObjectType::~ObjectType()
{
}

PUNICODE_STRING StarryEye::ObjectType::Name()
{
	return (PUNICODE_STRING)(NameOffset + address_);
}

bool StarryEye::ObjectType::IsProcess()
{
	return RtlCompareUnicodeString(Name(), &ProcessTypeString, TRUE) == 0;
}

bool StarryEye::ObjectType::IsThread()
{
	return RtlCompareUnicodeString(Name(), &ThreadTypeString, TRUE) == 0;
}
