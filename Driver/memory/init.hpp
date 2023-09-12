#pragma once
#include "control_area.h"
#include "subsection.h"

namespace StarryEye {
	void MemoryInit()
	{
		ControlArea::Init();
		SubSection::Init();
	}
}