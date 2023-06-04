#pragma once
#include "Config/kobject_base.h"

namespace StarryEye {
	class KThread: public KObjectBase
	{
	public:
		KThread(ULONG64 address);
		KThread(std::nullptr_t);
		~KThread();

	private:

	};
}