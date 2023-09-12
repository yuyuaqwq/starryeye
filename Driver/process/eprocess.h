#pragma once
#include "basic/structs.h"
#include "kprocess.h"
#include "vadtree.h"
#include "thread/ethread.h"
#include "handle/handle_table.h"

namespace StarryEye {
class EThread;

class EProcess: public KObject
{
public:
	static void Init();

	EProcess(const MmVirtualAddress& vaddr);
	EProcess() = default;
	~EProcess() = default;

	char* ImageFileName();
	KProcess Pcb();
	ListEntry<EProcess> ActiveProcessLinks();
	HandleTable ObjectTable();
	uint64_t InheritedFromUniqueProcessId();
	uint8_t OwnerProcessId();
	uint8_t PriorityClass();
	ListEntry<EThread> ThreadListHead();
	MmVadTree VadRoot();

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