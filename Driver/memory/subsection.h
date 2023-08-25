#pragma once
#include "memory/control_area.h"

namespace StarryEye {
	class SubSection: public KObjectBase
	{
	public:
		static void Init();

		SubSection(uint64_t address);
		SubSection(std::nullptr_t);
		~SubSection() = default;

		StarryEye::ControlArea ControlArea();
		SubSection NextSubsection();

	private:
		inline static uint64_t ControlAreaOffset;
		inline static uint64_t NextSubsectionOffset;
	};
}