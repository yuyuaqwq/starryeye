#pragma once
#include "Config/base.h"
#include "Handle/object_type.h"

namespace StarryEye {
class ObjectHeader: public KObjectBase
{
public:
	inline static UCHAR ObHeaderCookie;

	static void Init();
	static ULONG64 GetBodyOffset();

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
	inline static ULONG64 TypeIndexOffset;
	inline static ULONG64 BodyOffset;
};

template<class T>
inline T ObjectHeader::BodyObject()
{
	return T((ULONG64)Body());
}

}