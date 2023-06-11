#pragma once
#include "Config/base.h"

namespace StarryEye {
	class KThread: public KObjectBase
	{
	public:
		static void Init();

		KThread(ULONG64 address);
		KThread(std::nullptr_t);
		~KThread();

	private:

	};
}