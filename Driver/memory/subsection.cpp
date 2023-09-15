#include "subsection.h"

namespace stareye {
void SubSection::Init()
{
	ControlAreaOffset = 0;
	NextSubsectionOffset = 0x10;
}

SubSection::SubSection(const MmVirtualAddress& vaddr) : KObject(vaddr) {}

ControlArea SubSection::ControlArea()
{
	return (vaddr_ + ControlAreaOffset).ValU64();
}

SubSection SubSection::NextSubsection()
{
	return (vaddr_ + NextSubsectionOffset).ValU64();
}
}