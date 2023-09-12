#include "object_header.h"
#include "process/eprocess.h"

namespace StarryEye {
void ObjectHeader::Init()
{
	TypeIndexOffset = 0x18;
	BodyOffset = 0x30;
	ObHeaderCookie = (PUCHAR)0xfffff80260efc72c;	//TODO ObHeaderCookie
}

uint64_t ObjectHeader::GetBodyOffset() {
	return BodyOffset;
}

ObjectHeader::ObjectHeader(const MmVirtualAddress& vaddr): KObject(vaddr) {}

uint8_t ObjectHeader::TypeIndex()
{
	return (vaddr_ + TypeIndexOffset).ValU64();
}

uint8_t ObjectHeader::TypeIndexDecrypted()
{
	auto x = (uint8_t)(vaddr_.Address() >> 8);
	return TypeIndex() ^ x ^ (*ObHeaderCookie);
}

ObjectType ObjectHeader::Type()
{
	return ObjectType(ObjectType::ObTypeIndexTable[TypeIndexDecrypted()]);	// 根据TypeIndex从ObTypeIndexTable中获取_OBJECT_TYPE
}

bool ObjectHeader::IsProcess()
{
	return Type().CompareTypeName(L"Process");
}
}