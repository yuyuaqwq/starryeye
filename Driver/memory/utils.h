#pragma once

#include "basic/virtual_addr.h"

namespace stareye {
	using MmBuffer = krnlib::vector<char>;

	class MemoryUtils {
	public:
		enum PageProtection
		{
			kPageExecutable = 0b000,
			kPageReadWrite = 0b010,
			kPageCopyOnWrite = 0b100,
		};
		static MmBuffer AllocatePage(size_t size, uint32_t tag, PageProtection protection);
		static bool SetPageProtection(const MmVirtualAddress& address, PageProtection protection);
	};
}