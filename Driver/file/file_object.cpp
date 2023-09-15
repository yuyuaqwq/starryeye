#include "file_object.h"

namespace stareye {
FileObject::FileObject(const MmVirtualAddress& vaddr) : KObject(vaddr) {}

PUNICODE_STRING FileObject::FileName() {
	return &vaddr_.Pointer<FILE_OBJECT>()->FileName;
}
}