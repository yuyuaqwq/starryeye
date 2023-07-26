#pragma once
#include "Memory/control_area.h"

namespace StarryEye {
	class SubSection: public KObjectBase
	{
	public:
		static void Init();

		SubSection(ULONG64 address);
		SubSection(std::nullptr_t);
		~SubSection();

		StarryEye::ControlArea ControlArea();
		SubSection NextSubsection();

	private:
		inline static ULONG64 ControlAreaOffset;
		inline static ULONG64 NextSubsectionOffset;
	};
}