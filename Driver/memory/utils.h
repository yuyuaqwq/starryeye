#pragma once

#include "memory/page.h"

namespace StarryEye {
	class MemoryUtils {
	public:
		enum PageProtection
		{
			kPageExecutable = 0b000,
			kPageReadWrite = 0b010,
			kPageCopyOnWrite = 0b100,
		};
		static MmVirtualAddress AllocatePage(POOL_TYPE pool_type, size_t number_of_bytes, const char tag[4], PageProtection protection);
		static void SetPageProtection(const MmVirtualAddress& address, PageProtection protection);
	};
}