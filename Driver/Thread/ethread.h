#pragma once
#include "Config/base.h"
#include "Config/algorithm.h"
#include "Thread/kthread.h"

namespace StarryEye {
	class EThread: public KObjectBase
	{
	public:
		static void Init();
		static ULONG64 GetThreadListEntryOffset();

		EThread(ULONG64 address);
		EThread(std::nullptr_t);
		~EThread();

		KThread Tcb();
		KObjListEntry<EThread> ThreadListEntry();

	private:
		inline static ULONG64 ThreadListEntryOffset;
	};
}