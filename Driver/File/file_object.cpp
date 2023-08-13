#include "file/file_object.h"

namespace StarryEye {
FileObject::FileObject(uint64_t address)
	: KObjectBase(address & ~0xF),
	file_object_((PFILE_OBJECT)address_),
	property_((uint8_t)(address & 0xF)) {}
FileObject::FileObject(std::nullptr_t) : KObjectBase(nullptr), file_object_(nullptr), property_(0) {}
FileObject::~FileObject() {}

PUNICODE_STRING FileObject::FileName()
{
	return &file_object_->FileName;
}
}