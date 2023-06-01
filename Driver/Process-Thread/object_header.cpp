#include "object_header.h"

UCHAR StarryEye::ObjectHeader::DecryptTypeIndex(ULONG64 obj_addr, UCHAR type_index)
{
	return ObHeaderCookie ^ type_index ^ ((UCHAR*)obj_addr)[1];
}

StarryEye::ObjectHeader::ObjectHeader(std::nullptr_t)
{
	address_ = 0;
}

StarryEye::ObjectHeader::ObjectHeader(ULONG64 address)
{
	// TODO
	address_ = address;
}

StarryEye::ObjectHeader::~ObjectHeader()
{
	// TODO
}

UCHAR StarryEye::ObjectHeader::TypeIndex()
{
	return *(UCHAR*)(address_ + type_index_offset);
}

UCHAR StarryEye::ObjectHeader::TypeIndexDecrypted()
{
	return DecryptTypeIndex(address_, TypeIndex());
}

bool StarryEye::ObjectHeader::IsVaild()
{
	return MmIsAddressValid((PVOID)address_);
}

StarryEye::ObjectHeader::operator bool()
{
	return IsVaild();
}
