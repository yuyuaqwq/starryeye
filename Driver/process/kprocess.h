#pragma once
#include "config/base.h"
#include "config/algorithm.h"
#include "memory/page.h"

namespace StarryEye {
class KProcess: public KObjectBase
{
public:
	static void Init();

	KProcess(uint64_t address);
	KProcess(std::nullptr_t);
	~KProcess() = default;

	ListEntry ProcessListEntry();
	uint64_t DirectoryTableBase();

private:
	inline static uint64_t ProcessListEntryOffset;
	inline static uint64_t DirectoryTableBaseOffset;
};
}