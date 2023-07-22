#include "Memory/subsection.h"

namespace StarryEye {
	void SubSection::Init()
	{
		ControlAreaOffset = 0;
		NextSubsectionOffset = 0x10;
	}

	SubSection::SubSection(std::nullptr_t) : KObjectBase(nullptr) {}
	SubSection::SubSection(ULONG64 address) : KObjectBase(address) {}
	SubSection::~SubSection() {}

	ControlArea SubSection::ControlArea()
	{
		return StarryEye::ControlArea(*(PULONG64)(address_ + ControlAreaOffset));
	}

	SubSection SubSection::NextSubsection()
	{
		return SubSection(*(PULONG64)(address_ + NextSubsectionOffset));
	}
}