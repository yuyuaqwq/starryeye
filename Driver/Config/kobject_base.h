#pragma once
#include <ntifs.h>

namespace StarryEye {
	// 所有Kernel Object的基类, 主要是为了提供一个IsVaild函数和nullptr构造函数
	class KObjectBase
	{
	public:
		KObjectBase(ULONG64 address);
		KObjectBase(std::nullptr_t);
		~KObjectBase();

		bool IsVaild();

	protected:
		ULONG64 address_;
	};
}