#pragma once
#include "config/base.h"
#include "process/kprocess.h"

namespace StarryEye {
class KThread: public KObjectBase
{
public:
	static void Init();

	KThread(uint64_t address);
	KThread(std::nullptr_t);
	~KThread();

	KProcess Process();

private:
	inline static uint64_t ProcessOffset;
};
}