#pragma once
#include "process/kprocess.h"

namespace stareye {
class KThread: public KObject
{
public:
	static void Init();

	using KObject::KObject;

	KProcess Process();

private:
	inline static uint64_t ProcessOffset;
};
}