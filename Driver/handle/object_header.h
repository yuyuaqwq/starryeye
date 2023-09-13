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

	// ��ȡTypeIndex
	uint8_t TypeIndex() const;

	// ��ȡ���ܺ��TypeIndex
	uint8_t TypeIndexDecrypted() const;

	// ��ȡType����
	ObjectType Type() const;

	bool IsProcess() const;

	// ��ȡBody��ת��Ϊ����
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