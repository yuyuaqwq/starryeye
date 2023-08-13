#pragma once
#include "config/base.h"
#include "config/algorithm.h"

namespace StarryEye {
	class MmVadFlags: public KObjectBase
	{
	public:
		static void Init();

		MmVadFlags(uint64_t address);
		MmVadFlags(std::nullptr_t);
		~MmVadFlags();

		uint8_t PrivateMemory();

	private:
		static inline uint64_t PrivateMemoryBitPos;
	};
}