#pragma once
#include "Config/base.h"

namespace StarryEye {
	class ObjectType: public KObjectBase
	{
	public:
		inline static UNICODE_STRING ProcessTypeString;
		inline static UNICODE_STRING ThreadTypeString;
		inline static PULONG64 ObTypeIndexTable;

		static void Init();

		ObjectType(ULONG64 address);
		ObjectType(std::nullptr_t);
		~ObjectType();

		PUNICODE_STRING Name();

		bool IsProcess();
		bool IsThread();

	private:
		inline static ULONG64 NameOffset;
	};
}