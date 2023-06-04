#include "object_header.h"

UCHAR StarryEye::ObjectHeader::DecryptTypeIndex(ULONG64 obj_addr, UCHAR type_index)
{
	UCHAR x = (UCHAR)(obj_addr >> 8);
	return 0xd3 ^ type_index ^ x;
}

StarryEye::ObjectHeader::ObjectHeader(std::nullptr_t) : KObjectBase(nullptr)
{
	type_index_offset = 0;
	body_offset_ = 0;
}

StarryEye::ObjectHeader::ObjectHeader(ULONG64 address) : KObjectBase(address)
{
	type_index_offset = 0x18;		//TODO TypeIndex偏移
	body_offset_ = 0x30;
}

StarryEye::ObjectHeader::~ObjectHeader()
{
}

UCHAR StarryEye::ObjectHeader::TypeIndex()
{
	return *(UCHAR*)(address_ + type_index_offset);
}

UCHAR StarryEye::ObjectHeader::TypeIndexDecrypted()
{
	return DecryptTypeIndex(address_, TypeIndex());
}

StarryEye::ObjectType StarryEye::ObjectHeader::Type()
{
	return ObjectType(ObTypeIndexTable[TypeIndexDecrypted()]);	// 根据TypeIndex从ObTypeIndexTable中获取_OBJECT_TYPE
}

PVOID StarryEye::ObjectHeader::Body()
{
	return (PVOID)(address_ + body_offset_);
}
