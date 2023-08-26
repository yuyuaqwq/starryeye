#pragma once
#include "config/base.h"
#include "handle/object_type.h"
#include <fustd/generic/option.hpp>

namespace StarryEye {
class EProcess;

class ObjectHeader: public KObjectBase
{
public:
	inline static PUCHAR ObHeaderCookie;

	static void Init();
	static uint64_t GetBodyOffset();

	// 解密TypeIndex
	static uint8_t DecryptTypeIndex(uint64_t obj_addr, uint8_t type_index);

	ObjectHeader() = default;
	ObjectHeader(uint64_t address);
	~ObjectHeader() = default;

	// 获取TypeIndex
	uint8_t TypeIndex();

	// 获取解密后的TypeIndex
	uint8_t TypeIndexDecrypted();

	// 获取Type对象
	ObjectType Type();

	bool IsProcess();

	fustd::Option<EProcess> ConvToEProc();

	// 获取Body并转换为对象
	template<class T>
	T Body();

private:
	inline static uint64_t TypeIndexOffset;
	inline static uint64_t BodyOffset;
};

template<class T>
inline T ObjectHeader::Body()
{
	return T(address_ + BodyOffset);
}

}