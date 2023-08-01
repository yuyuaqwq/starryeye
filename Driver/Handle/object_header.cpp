#include "object_header.h"
#include "Process/eprocess.h"

namespace StarryEye {
void ObjectHeader::Init()
{
	TypeIndexOffset = 0x18;
	BodyOffset = 0x30;
	ObHeaderCookie = (PUCHAR)0xfffff80260efc72c;	//TODO ObHeaderCookie
}

ULONG64 ObjectHeader::GetBodyOffset()
{
	return BodyOffset;
}

UCHAR ObjectHeader::DecryptTypeIndex(ULONG64 obj_addr, UCHAR type_index)
{
	auto x = (UCHAR)(obj_addr >> 8);
	return type_index ^ x ^ (*ObHeaderCookie);
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

bool ObjectHeader::IsProcess()
{
	return Type().CompareTypeName(L"Process");
}
krnlib::Option<EProcess> ObjectHeader::ConvToEProc()
{
	if (IsProcess())
		return krnlib::Some(Body<EProcess>());
	else
		return krnlib::None();
}
}