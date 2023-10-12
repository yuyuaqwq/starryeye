#pragma once
#include "basic/structs.h"
#include "kthread.h"
#include "process/eprocess.h"

namespace stareye {
class EThread: public KObject
{
	STAREYE_USING_BASE(KObject)
public:
	static void Init();

	KThread Tcb();
	ListEntry<EThread> ThreadListEntry();

private:
	friend class EProcess;

	inline static uint64_t ThreadListEntryOffset;
};
}