#pragma once
#include "Config/base.h"
#include "Process/kprocess.h"

namespace StarryEye {
class KThread: public KObjectBase
{
public:
	static void Init();

	KThread(ULONG64 address);
	KThread(std::nullptr_t);
	~KThread();

	KProcess Process();

private:
	inline static ULONG64 ProcessOffset;
};
}