#pragma once
#include "memory/control_area.h"

namespace StarryEye {
	class SubSection: public KObject
	{
	public:
		static void Init();

		SubSection(const MmVirtualAddress& vaddr);
		SubSection() = default;
		~SubSection() = default;

		StarryEye::ControlArea ControlArea();
		SubSection NextSubsection();

	private:
		inline static uint64_t ControlAreaOffset;
		inline static uint64_t NextSubsectionOffset;
	};
}