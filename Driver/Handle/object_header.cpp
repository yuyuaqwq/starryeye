#include "object_header.h"

using namespace StarryEye;

void StarryEye::ObjectHeader::Init()
{
	TypeIndexOffset = 0x18;		//TODO TypeIndex偏移
	BodyOffset = 0x30;
	ObHeaderCookie = 0xd3;
}

ULONG64 StarryEye::ObjectHeader::GetBodyOffset()
{
	return BodyOffset;
}

UCHAR StarryEye::ObjectHeader::DecryptTypeIndex(ULONG64 obj_addr, UCHAR type_index)
{
	UCHAR x = (UCHAR)(obj_addr >> 8);
	return 0xd3 ^ type_index ^ x;
}

StarryEye::ObjectHeader::ObjectHeader(std::nullptr_t) : KObjectBase(nullptr)
{
}

StarryEye::ObjectHeader::ObjectHeader(ULONG64 address) : KObjectBase(address)
{
}

StarryEye::ObjectHeader::~ObjectHeader()
{
}

UCHAR StarryEye::ObjectHeader::TypeIndex()
{
	return *(UCHAR*)(address_ + TypeIndexOffset);
}

UCHAR StarryEye::ObjectHeader::TypeIndexDecrypted()
{
	return DecryptTypeIndex(address_, TypeIndex());
}

ObjectType StarryEye::ObjectHeader::Type()
{
	return ObjectType(ObjectType::ObTypeIndexTable[TypeIndexDecrypted()]);	// 根据TypeIndex从ObTypeIndexTable中获取_OBJECT_TYPE
}

PVOID StarryEye::ObjectHeader::Body()
{
	return (PVOID)(address_ + BodyOffset);
}
