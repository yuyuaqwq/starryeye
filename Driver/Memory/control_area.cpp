#include "control_area.h"

namespace StarryEye {
void ControlArea::Init()
{
	FilePointerOffset = 0x40;
}

ControlArea::ControlArea(std::nullptr_t) : KObjectBase(nullptr) {}
ControlArea::ControlArea(ULONG64 address) : KObjectBase(address) {}
ControlArea::~ControlArea() {}

FileObject ControlArea::FilePointer()
{
	return FileObject(*(PULONG64)(address_ + FilePointerOffset));
}
}