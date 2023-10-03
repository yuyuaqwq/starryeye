#pragma once
#include "process/kprocess.h"

namespace stareye {
class KThread: public KObject
{
public:
	static void Init();

	KThread(MmVirtualAddress vaddr);
	KThread() = default;
	~KThread() = default;

	KProcess Process();

private:
	inline static uint64_t ProcessOffset;
};
}