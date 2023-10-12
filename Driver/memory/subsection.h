#pragma once
#include "control_area.h"

namespace stareye {
	class SubSection: public KObject
	{
		STAREYE_USING_BASE(KObject)
	public:
		static void Init();

		ControlArea ControlArea();
		SubSection NextSubsection();

	private:
		inline static uint64_t ControlAreaOffset;
		inline static uint64_t NextSubsectionOffset;
	};
}