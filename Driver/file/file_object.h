#pragma once
#include "basic/structs.h"

namespace StarryEye {
	class FileObject: public KObject
	{
	public:
		FileObject(const MmVirtualAddress& vaddr);
		FileObject() = default;
		~FileObject() = default;

		PUNICODE_STRING FileName();
	};
}