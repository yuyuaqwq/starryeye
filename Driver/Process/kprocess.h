#pragma once
#include "Config/base.h"
#include "Config/algorithm.h"

namespace StarryEye {
	class KProcess: public KObjectBase
	{
	public:
		static void Init();

		KProcess(ULONG64 address);
		KProcess(std::nullptr_t);
		~KProcess();

		KObjListEntry<KProcess> ProcessListEntry();

	private:
		inline static ULONG64 ProcessListEntryOffset;
	};
}