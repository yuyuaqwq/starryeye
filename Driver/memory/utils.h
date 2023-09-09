#pragma once

#include "memory/virtual_addr.h"

namespace StarryEye {
	class MemoryUtils {
	public:
		enum PageProtection
		{
			kPageExecutable = 0b000,
			kPageReadWrite = 0b010,
			kPageCopyOnWrite = 0b100,
		};
		static MmVirtualAddress AllocatePage(POOL_TYPE pool_type, size_t number_of_bytes, uint32_t tag, PageProtection protection);
		static bool SetPageProtection(const MmVirtualAddress& address, PageProtection protection);
	};
}