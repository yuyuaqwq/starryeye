#include "object_header.h"
#include "process/eprocess.h"

namespace stareye {
void ObjectHeader::Init()
{
	TypeIndexOffset = 0x18;
	BodyOffset = 0x30;
	ObHeaderCookie = 0xfffff802598fc72c;	//TODO ObHeaderCookie
}

uint8_t ObjectHeader::TypeIndex() const
{
	return (vaddr_ + TypeIndexOffset).ValU64();
}

uint8_t ObjectHeader::TypeIndexDecrypted() const
{
	auto x = (uint8_t)(vaddr_.Address() >> 8);
	return TypeIndex() ^ x ^ (ObHeaderCookie.ValU8());
}

ObjectType ObjectHeader::Type() const
{
	return ObjectType(ObjectType::ObTypeIndexTable.PtrU64()[TypeIndexDecrypted()]);	// 根据TypeIndex从ObTypeIndexTable中获取_OBJECT_TYPE
}

bool ObjectHeader::IsProcess() const
{
	return Type().CompareTypeName(L"Process");
}
}