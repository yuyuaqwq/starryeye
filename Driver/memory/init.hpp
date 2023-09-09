#pragma once
#include "memory/control_area.h"
#include "memory/virtual_addr.h"
#include "memory/subsection.h"

namespace StarryEye {
	void MemoryInit()
	{
		ControlArea::Init();
		SubSection::Init();
		MmVirtualAddress::Init();
	}
}