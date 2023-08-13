#pragma once
#include "config/base.h"

namespace StarryEye {
	class FileObject: public KObjectBase
	{
	public:
		FileObject(uint64_t address);
		FileObject(std::nullptr_t);
		~FileObject();

		PUNICODE_STRING FileName();

	private:
		PFILE_OBJECT file_object_;
		uint8_t property_;
	};
}