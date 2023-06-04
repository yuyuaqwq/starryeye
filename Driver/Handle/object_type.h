#pragma once
#include "Config/kobject_base.h"
#include "Config/global_vars.h"

namespace StarryEye {
	class ObjectType: public KObjectBase
	{
	public:
		ObjectType(ULONG64 address);
		ObjectType(std::nullptr_t);
		~ObjectType();

		PUNICODE_STRING Name();

		bool IsProcess();
		bool IsThread();

	private:
		UINT16 name_offset_;
	};
}