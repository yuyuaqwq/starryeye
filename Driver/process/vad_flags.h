#pragma once
#include "basic/structs.h"

namespace StarryEye {
class MmVadFlags: public KObject
{
public:
	static void Init();

	MmVadFlags(const MmVirtualAddress& vaddr);
	MmVadFlags() = default;
	~MmVadFlags() = default;

	uint8_t PrivateMemory();

private:
	static inline uint64_t PrivateMemoryBitPos;
	static inline uint64_t PrivateMemoryBitSize;
};
}