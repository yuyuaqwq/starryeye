#pragma once
#include "config/base.h"
#include "config/algorithm.h"

namespace StarryEye {
class KProcess: public KObjectBase
{
public:
	static void Init();

	KProcess(uint64_t address);
	KProcess(std::nullptr_t);
	~KProcess();

	ListEntry ProcessListEntry();

private:
	inline static uint64_t ProcessListEntryOffset;
};
}