#pragma once
#include "Config/kobject_base.h"
#include "object_type.h"
#include "Config/global_vars.h"

namespace StarryEye {

#define OBJECT_HEADER_TO_BODY_SIZE 0x30


class ObjectHeader: public KObjectBase
{
public:
	// 解密TypeIndex
	static UCHAR DecryptTypeIndex(ULONG64 obj_addr, UCHAR type_index);

	ObjectHeader(std::nullptr_t);
	ObjectHeader(ULONG64 address);
	~ObjectHeader();

	// 获取TypeIndex
	UCHAR TypeIndex();

	// 获取解密后的TypeIndex
	UCHAR TypeIndexDecrypted();

	// 获取Type对象(注意: 使用前一定要先调用ObjectType::Init()!)
	ObjectType Type();

	// 获取Body
	PVOID Body();

	// 获取Body并转换为对象
	template<class T>
	T BodyObject();

private:
	UINT16 type_index_offset;
	UINT16 body_offset_;
};

template<class T>
inline T ObjectHeader::BodyObject()
{
	return T((ULONG64)Body());
}

}