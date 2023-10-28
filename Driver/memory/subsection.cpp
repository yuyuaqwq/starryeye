#include "subsection.h"

namespace stareye {
void SubSection::Init()
{
	ControlAreaOffset = 0;
	NextSubsectionOffset = 0x10;
}

ControlArea SubSection::ControlArea()
{
	return (vaddr_ + ControlAreaOffset).DerefAsAddr();
}

SubSection SubSection::NextSubsection()
{
	return (vaddr_ + NextSubsectionOffset).DerefAsAddr();
}
}