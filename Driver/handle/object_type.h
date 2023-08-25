#pragma once
#include "config/base.h"

namespace StarryEye {
	class ObjectType: public KObjectBase
	{
	public:
		inline static uint64_t* ObTypeIndexTable;

		static void Init();

		ObjectType(uint64_t address);
		ObjectType(std::nullptr_t);
		~ObjectType() = default;

		PUNICODE_STRING Name();

		bool CompareTypeName(PCWSTR name);

	private:
		inline static uint64_t NameOffset;
	};
}