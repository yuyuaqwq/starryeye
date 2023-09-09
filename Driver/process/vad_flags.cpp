#include "vad_flags.h"

namespace StarryEye {
	void MmVadFlags::Init()
	{
		PrivateMemoryBitPos = 20;
		PrivateMemoryBitSize = 1;
	}

	MmVadFlags::MmVadFlags(uint64_t address) : KObjectBase(address) {}

	uint8_t MmVadFlags::PrivateMemory()
	{
		return GetBitAreaValue((PVOID)address_, 4, PrivateMemoryBitPos, PrivateMemoryBitSize).Default(0);
	}
}