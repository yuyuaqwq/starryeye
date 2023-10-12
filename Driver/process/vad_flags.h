#pragma once
#include "basic/structs.h"

namespace stareye {
class MmVadFlags: public KObject
{
	STAREYE_USING_BASE(KObject)
public:
	static void Init();

	uint8_t PrivateMemory();

private:
	static inline uint64_t PrivateMemoryBitPos;
	static inline uint64_t PrivateMemoryBitSize;
};
}