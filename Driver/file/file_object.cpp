#include "file_object.h"

namespace stareye {
PUNICODE_STRING FileObject::FileName() {
	return &vaddr_.Pointer<FILE_OBJECT>()->FileName;
}
}