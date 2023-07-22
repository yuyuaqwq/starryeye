#include "File/file_object.h"

namespace StarryEye {
	FileObject::FileObject(ULONG64 address) : KObjectBase(address), file_object_((PFILE_OBJECT)address) {}
	FileObject::FileObject(std::nullptr_t) : KObjectBase(nullptr), file_object_(nullptr) {}
	FileObject::~FileObject() {}

	PUNICODE_STRING FileObject::FileName()
	{
		return &file_object_->FileName;
	}
}