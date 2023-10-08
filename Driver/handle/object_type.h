#pragma once
#include "basic/structs.h"

namespace stareye {
	class ObjectType: public KObject
	{
	public:
		inline static MmVirtualAddress ObTypeIndexTable;

		static void Init();

		using KObject::KObject;

		PUNICODE_STRING Name();

		bool CompareTypeName(PCWSTR name);

	private:
		inline static uint64_t NameOffset;
	};
}