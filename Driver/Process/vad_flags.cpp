#include "vad_flags.h"

namespace StarryEye {
	void MmVadFlags::Init()
	{
		PrivateMemoryBitPos = 20;
	}

	MmVadFlags::MmVadFlags(std::nullptr_t) : KObjectBase(nullptr) {}
	MmVadFlags::MmVadFlags(ULONG64 address) : KObjectBase(address) {}
	MmVadFlags::~MmVadFlags() {}

	UCHAR MmVadFlags::PrivateMemory()
	{
		if (auto res = GetBitAreaValue((PVOID)address_, PrivateMemoryBitPos, 1); res.Ok())
			return res.OkVal();
		return 0;
	}
}