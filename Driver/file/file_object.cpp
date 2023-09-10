#include "file/file_object.h"

namespace StarryEye {
FileObject::FileObject(const MmVirtualAddress& vaddr, uint8_t property) : KObject(vaddr), property_(property) {}

PUNICODE_STRING FileObject::FileName() {
	return &vaddr_.PtrUnsafe<FILE_OBJECT>()->FileName;
}
}