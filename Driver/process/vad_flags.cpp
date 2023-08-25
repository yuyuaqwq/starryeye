#include "vad_flags.h"

namespace StarryEye {
	void MmVadFlags::Init()
	{
		PrivateMemoryBitPos = 20;
		PrivateMemoryBitSize = 1;
	}

	MmVadFlags::MmVadFlags(std::nullptr_t) : KObjectBase(nullptr) {}
	MmVadFlags::MmVadFlags(uint64_t address) : KObjectBase(address) {}

	uint8_t MmVadFlags::PrivateMemory()
	{
		if (auto res = GetBitAreaValue((PVOID)address_, 4, PrivateMemoryBitPos, PrivateMemoryBitSize); res.IsSome())
			return res.SomeVal();
		return 0;
	}
}