#include "control_area.h"

namespace StarryEye {
void ControlArea::Init()
{
	FilePointerOffset = 0x40;
}

ControlArea::ControlArea(std::nullptr_t) : KObjectBase(nullptr) {}
ControlArea::ControlArea(uint64_t address) : KObjectBase(address) {}

FileObject ControlArea::FilePointer()
{
	return FileObject(*(uint64_t*)(address_ + FilePointerOffset));
}
}