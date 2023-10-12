#pragma once
#include "basic/structs.h"

namespace stareye {
	class ObjectType: public KObject
	{
		STAREYE_USING_BASE(KObject)
	public:
		inline static MmVirtualAddress ObTypeIndexTable;

		static void Init();

		PUNICODE_STRING Name();

		bool CompareTypeName(PCWSTR name);

	private:
		inline static uint64_t NameOffset;
	};
}