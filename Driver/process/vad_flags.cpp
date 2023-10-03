#include "vad_flags.h"

namespace stareye {
void MmVadFlags::Init()
{
	PrivateMemoryBitPos = 20;
	PrivateMemoryBitSize = 1;
}

MmVadFlags::MmVadFlags(MmVirtualAddress vaddr) : KObject(vaddr) {}

uint8_t MmVadFlags::PrivateMemory()
{
	return vaddr_.BitArea(PrivateMemoryBitPos, PrivateMemoryBitSize);
}
}