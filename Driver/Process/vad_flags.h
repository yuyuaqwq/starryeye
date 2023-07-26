#pragma once
#include "Config/base.h"
#include "Config/algorithm.h"

namespace StarryEye {
	class MmVadFlags: public KObjectBase
	{
	public:
		static void Init();

		MmVadFlags(ULONG64 address);
		MmVadFlags(std::nullptr_t);
		~MmVadFlags();

		UCHAR PrivateMemory();

	private:
		static inline ULONG64 PrivateMemoryBitPos;
	};
}