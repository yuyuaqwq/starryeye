#pragma once
#include "control_area.h"

namespace stareye {
	class SubSection: public KObject
	{
	public:
		static void Init();

		SubSection(MmVirtualAddress vaddr);
		SubSection() = default;
		~SubSection() = default;

		ControlArea ControlArea();
		SubSection NextSubsection();

	private:
		inline static uint64_t ControlAreaOffset;
		inline static uint64_t NextSubsectionOffset;
	};
}