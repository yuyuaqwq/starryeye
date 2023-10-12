#pragma once
#include "process/kprocess.h"

namespace stareye {
class KThread: public KObject
{
	STAREYE_USING_BASE(KObject)
public:
	static void Init();

	KProcess Process();

private:
	inline static uint64_t ProcessOffset;
};
}