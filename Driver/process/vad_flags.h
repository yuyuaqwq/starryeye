#pragma once
#include "basic/structs.h"

namespace stareye {
class MmVadFlags: public KObject
{
public:
	static void Init();

	using KObject::KObject;

	uint8_t PrivateMemory();

private:
	static inline uint64_t PrivateMemoryBitPos;
	static inline uint64_t PrivateMemoryBitSize;
};
}