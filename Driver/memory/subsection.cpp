#include "subsection.h"

namespace stareye {
void SubSection::Init()
{
	ControlAreaOffset = 0;
	NextSubsectionOffset = 0x10;
}

SubSection::SubSection(MmVirtualAddress vaddr) : KObject(vaddr) {}

ControlArea SubSection::ControlArea()
{
	return stareye::ControlArea((vaddr_ + ControlAreaOffset).ValU64());
}

SubSection SubSection::NextSubsection()
{
	return SubSection((vaddr_ + NextSubsectionOffset).ValU64());
}
}