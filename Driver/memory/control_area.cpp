#include "control_area.h"

namespace StarryEye {
void ControlArea::Init()
{
	FilePointerOffset = 0x40;
}

ControlArea::ControlArea(const MmVirtualAddress& vaddr) : KObject(vaddr) {}

ExFastRef<FileObject> ControlArea::FilePointer() {
	return vaddr_ + FilePointerOffset;
}
}