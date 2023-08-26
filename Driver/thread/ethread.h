#pragma once
#include "config/base.h"
#include "config/algorithm.h"
#include "thread/kthread.h"
#include "process/eprocess.h"

namespace StarryEye {
class EThread: public KObjectBase
{
public:
	static void Init();

	EThread(uint64_t address);
	EThread() = default;
	~EThread() = default;

	KThread Tcb();
	ListEntry ThreadListEntry();

private:
	friend class EProcess;

	inline static uint64_t ThreadListEntryOffset;
};
}