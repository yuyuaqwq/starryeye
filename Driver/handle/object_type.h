#pragma once
#include "basic/structs.h"

namespace StarryEye {
	class ObjectType: public KObject
	{
	public:
		inline static uint64_t* ObTypeIndexTable;

		static void Init();

		ObjectType(const MmVirtualAddress& vaddr);
		ObjectType() = default;
		~ObjectType() = default;

		PUNICODE_STRING Name();

		bool CompareTypeName(PCWSTR name);

	private:
		inline static uint64_t NameOffset;
	};
}