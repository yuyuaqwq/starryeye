#include "object_header.h"
#include "process/eprocess.h"

namespace StarryEye {
void ObjectHeader::Init()
{
	TypeIndexOffset = 0x18;
	BodyOffset = 0x30;
	ObHeaderCookie = (PUCHAR)0xfffff80260efc72c;	//TODO ObHeaderCookie
}

uint64_t ObjectHeader::GetBodyOffset()
{
	return BodyOffset;
}

uint8_t ObjectHeader::DecryptTypeIndex(uint64_t obj_addr, uint8_t type_index)
{
	auto x = (uint8_t)(obj_addr >> 8);
	return type_index ^ x ^ (*ObHeaderCookie);
}

ObjectHeader::ObjectHeader(std::nullptr_t) : KObjectBase(nullptr) {}
ObjectHeader::ObjectHeader(uint64_t address) : KObjectBase(address) {}
ObjectHeader::~ObjectHeader() {}

uint8_t ObjectHeader::TypeIndex()
{
	return *(uint8_t*)(address_ + TypeIndexOffset);
}

uint8_t ObjectHeader::TypeIndexDecrypted()
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
fustd::Option<EProcess> ObjectHeader::ConvToEProc()
{
	if (IsProcess())
		return fustd::Some(Body<EProcess>());
	else
		return fustd::None();
}
}