#pragma once
#include "basic/structs.h"
#include "handle/object_type.h"
#include <fustd/generic/option.hpp>

namespace StarryEye {
class EProcess;

class ObjectHeader: public KObject
{
public:
	inline static PUCHAR ObHeaderCookie;

	static void Init();
	static uint64_t GetBodyOffset();

	ObjectHeader() = default;
	ObjectHeader(const MmVirtualAddress& vaddr);
	~ObjectHeader() = default;

	// ��ȡTypeIndex
	uint8_t TypeIndex();

	// ��ȡ���ܺ��TypeIndex
	uint8_t TypeIndexDecrypted();

	// ��ȡType����
	ObjectType Type();

	bool IsProcess();

	// ��ȡBody��ת��Ϊ����
	template<class T, EnableIfInheritKObject<T> = 0>
	T Body() {
		return vaddr_ + BodyOffset;
	}

private:
	inline static uint64_t TypeIndexOffset;
	inline static uint64_t BodyOffset;
};

}