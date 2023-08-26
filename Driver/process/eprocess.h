#pragma once
#include "config/base.h"
#include "config/algorithm.h"
#include "process/kprocess.h"
#include "thread/ethread.h"
#include "handle/handle_table.h"

namespace StarryEye {
class EThread;
class VadTree;

class EProcess: public KObjectBase
{
public:
	static void Init();

	EProcess(uint64_t address);
	EProcess() = default;
	~EProcess() = default;

	PCHAR ImageFileName();
	KProcess Pcb();
	ListEntry ActiveProcessLinks();
	HandleTable ObjectTable();
	uint64_t InheritedFromUniqueProcessId();
	UINT8 OwnerProcessId();
	uint8_t PriorityClass();
	ListEntry ThreadListHead();
	VadTree VadRoot();

	bool CompareFileName(PCCHAR file_name);

private:
	inline static uint64_t ImageFileNameOffset;
	inline static uint64_t ObjectTableOffset;
	inline static uint64_t ThreadListHeadOffset;
	inline static uint64_t OwnerProcessIdOffset;
	inline static uint64_t PriorityClassOffset;
	inline static uint64_t ActiveProcessLinksOffset;
	inline static uint64_t InheritedFromUniqueProcessIdOffset;
	inline static uint64_t VadRootOffset;
};
}