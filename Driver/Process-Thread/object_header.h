#pragma once
#include <ntifs.h>

namespace StarryEye {

#define OBJECT_HEADER_TO_BODY_SIZE 0x30

static ULONG64 ObHeaderCookie;

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

	bool IsVaild();

	operator bool();

private:
	// 地址
	ULONG64 address_;
	UINT16 type_index_offset;
};

}