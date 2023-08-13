#include "subsection.h"

namespace StarryEye {
void SubSection::Init()
{
	ControlAreaOffset = 0;
	NextSubsectionOffset = 0x10;
}

SubSection::SubSection(std::nullptr_t) : KObjectBase(nullptr) {}
SubSection::SubSection(uint64_t address) : KObjectBase(address) {}
SubSection::~SubSection() {}

ControlArea SubSection::ControlArea()
{
	return StarryEye::ControlArea(*(uint64_t*)(address_ + ControlAreaOffset));
}

SubSection SubSection::NextSubsection()
{
	return SubSection(*(uint64_t*)(address_ + NextSubsectionOffset));
}
}