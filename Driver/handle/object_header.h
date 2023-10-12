#pragma once
#include "basic/structs.h"
#include "object_type.h"

namespace stareye {
class HandleTable;

class ObjectHeader: public KObject
{
	STAREYE_USING_BASE(KObject)
public:
	inline static MmVirtualAddress ObHeaderCookie;

	static void Init();

	// 获取TypeIndex
	uint8_t TypeIndex() const;

	// 获取解密后的TypeIndex
	uint8_t TypeIndexDecrypted() const;

	// 获取Type对象
	ObjectType Type() const;

	bool IsProcess() const;

	// 获取Body并转换为对象
	template<InheritKObjectT T>
	T Body() const {
		return vaddr_ + BodyOffset;
	}

private:
	friend class HandleTable;

	inline static uint64_t TypeIndexOffset;
	inline static uint64_t BodyOffset;
};

}