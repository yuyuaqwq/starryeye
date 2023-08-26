#include "subsection.h"

namespace StarryEye {
void SubSection::Init()
{
	ControlAreaOffset = 0;
	NextSubsectionOffset = 0x10;
}

SubSection::SubSection(uint64_t address) : KObjectBase(address) {}

ControlArea SubSection::ControlArea()
{
	return StarryEye::ControlArea(*(uint64_t*)(address_ + ControlAreaOffset));
}

SubSection SubSection::NextSubsection()
{
	return SubSection(*(uint64_t*)(address_ + NextSubsectionOffset));
}
}