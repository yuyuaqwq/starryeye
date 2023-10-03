#include "control_area.h"

namespace stareye {
void ControlArea::Init()
{
	FilePointerOffset = 0x40;
}

ControlArea::ControlArea(MmVirtualAddress vaddr) : KObject(vaddr) {}

ExFastRef<FileObject> ControlArea::FilePointer() {
	return vaddr_ + FilePointerOffset;
}
}