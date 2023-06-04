#pragma once
#include "Config/kobject_base.h"

namespace StarryEye {
	class KProcess: public KObjectBase
	{
	public:
		KProcess(ULONG64 address);
		KProcess(std::nullptr_t);
		~KProcess();

	private:

	};
}