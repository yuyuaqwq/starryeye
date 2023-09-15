#pragma once
#include "control_area.h"
#include "subsection.h"

namespace stareye {
	void MemoryInit()
	{
		ControlArea::Init();
		SubSection::Init();
	}
}