#pragma once
#include <ntifs.h>
#include "object_type.h"
#include "global_vars.hpp"

namespace StarryEye {

#define OBJECT_HEADER_TO_BODY_SIZE 0x30


class ObjectHeader
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

	bool IsVaild();

private:
	// 地址
	ULONG64 address_;
	UINT16 type_index_offset;
};

}