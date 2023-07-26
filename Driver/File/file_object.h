#pragma once
#include "Config/base.h"

namespace StarryEye {
	class FileObject: public KObjectBase
	{
	public:
		FileObject(ULONG64 address);
		FileObject(std::nullptr_t);
		~FileObject();

		PUNICODE_STRING FileName();

	private:
		PFILE_OBJECT file_object_;
		UCHAR property_;
	};
}