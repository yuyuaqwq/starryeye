#pragma once
#include "basic/structs.h"

namespace StarryEye {
class KProcess: public KObject
{
public:
	static void Init();

	KProcess(const MmVirtualAddress& vaddr);
	KProcess() = default;
	~KProcess() = default;

	ListEntry<KProcess> ProcessListEntry();
	uint64_t DirectoryTableBase();

private:
	inline static uint64_t ProcessListEntryOffset;
	inline static uint64_t DirectoryTableBaseOffset;
};
}