#pragma once
#include "Config/kobject_base.h"

namespace StarryEye {
	class EThread: public KObjectBase
	{
	public:
		EThread(ULONG64 address);
		EThread(std::nullptr_t);
		~EThread();

	private:
	};
}