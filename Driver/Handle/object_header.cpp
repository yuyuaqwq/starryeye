#include "object_header.h"

namespace StarryEye {
void ObjectHeader::Init()
{
	TypeIndexOffset = 0x18;
	BodyOffset = 0x30;
	ObHeaderCookie = 0xd3;
}

ULONG64 ObjectHeader::GetBodyOffset()
{
	return BodyOffset;
}

UCHAR ObjectHeader::DecryptTypeIndex(ULONG64 obj_addr, UCHAR type_index)
{
	UCHAR x = (UCHAR)(obj_addr >> 8);
	return 0xd3 ^ type_index ^ x;
}

ObjectHeader::ObjectHeader(std::nullptr_t) : KObjectBase(nullptr) {}
ObjectHeader::ObjectHeader(ULONG64 address) : KObjectBase(address) {}
ObjectHeader::~ObjectHeader() {}

UCHAR ObjectHeader::TypeIndex()
{
	return *(UCHAR*)(address_ + TypeIndexOffset);
}

UCHAR ObjectHeader::TypeIndexDecrypted()
{
	return DecryptTypeIndex(address_, TypeIndex());
}

ObjectType ObjectHeader::Type()
{
	return ObjectType(ObjectType::ObTypeIndexTable[TypeIndexDecrypted()]);	// 根据TypeIndex从ObTypeIndexTable中获取_OBJECT_TYPE
}

PVOID ObjectHeader::Body()
{
	return (PVOID)(address_ + BodyOffset);
}
}