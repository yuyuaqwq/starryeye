#pragma once
#include "basic/structs.h"
#include "kthread.h"
#include "process/eprocess.h"

namespace StarryEye {
class EThread: public KObject
{
public:
	static void Init();

	EThread(const MmVirtualAddress& vaddr);
	EThread() = default;
	~EThread() = default;

	KThread Tcb();
	ListEntry<EThread> ThreadListEntry();

private:
	friend class EProcess;

	inline static uint64_t ThreadListEntryOffset;
};
}