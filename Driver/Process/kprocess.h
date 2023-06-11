#pragma once
#include "Config/base.h"

namespace StarryEye {
	class KProcess: public KObjectBase
	{
	public:
		static void Init();

		KProcess(ULONG64 address);
		KProcess(std::nullptr_t);
		~KProcess();

	private:

	};
}