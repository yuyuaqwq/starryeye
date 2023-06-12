#pragma once
#include "Config/base.h"
#include "Config/algorithm.h"
#include "Thread/kthread.h"
#include "Process/eprocess.h"

namespace StarryEye {
	class EThread: public KObjectBase
	{
	public:
		static void Init();

		EThread(ULONG64 address);
		EThread(std::nullptr_t);
		~EThread();

		KThread Tcb();
		KObjListEntry<EThread> ThreadListEntry();

	private:
		friend class EProcess;

		inline static ULONG64 ThreadListEntryOffset;
	};
}