#pragma once
#include "memory/control_area.h"
#include "memory/subsection.h"

namespace StarryEye {
	void MemoryInit()
	{
		ControlArea::Init();
		SubSection::Init();
	}
}