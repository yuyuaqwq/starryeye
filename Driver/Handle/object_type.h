#pragma once
#include <ntifs.h>
#include "global_vars.hpp"

namespace StarryEye {
	class ObjectType
	{
	public:
		ObjectType(ULONG64 address);
		ObjectType(std::nullptr_t);
		~ObjectType();

		bool IsValid();

		PUNICODE_STRING Name();

		bool IsProcess();
		bool IsThread();

	private:
		ULONG64 address_;
		UINT16 name_offset_;
	};
}