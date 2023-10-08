#include "control_area.h"

namespace stareye {
void ControlArea::Init()
{
	FilePointerOffset = 0x40;
}

ExFastRef<FileObject> ControlArea::FilePointer() {
	return vaddr_ + FilePointerOffset;
}
}