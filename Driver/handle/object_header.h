#pragma once
#include "basic/structs.h"
#include "object_type.h"
#include <fustd/generic/option.hpp>

namespace StarryEye {
class HandleTable;

class ObjectHeader: public KObject
{
public:
	inline static MmVirtualAddress ObHeaderCookie;

	static void Init();

	ObjectHeader() = default;
	ObjectHeader(const MmVirtualAddress& vaddr);
	~ObjectHeader() = default;

	// 获取TypeIndex
	uint8_t TypeIndex() const;

	// 获取解密后的TypeIndex
	uint8_t TypeIndexDecrypted() const;

	// 获取Type对象
	ObjectType Type() const;

	bool IsProcess() const;

	// 获取Body并转换为对象
	template<class T, EnableIfInheritKObject<T> = 0>
	T Body() const {
		return vaddr_ + BodyOffset;
	}

private:
	friend class HandleTable;

	inline static uint64_t TypeIndexOffset;
	inline static uint64_t BodyOffset;
};

}