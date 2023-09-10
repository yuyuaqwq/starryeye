#pragma once
#include "config/structs.h"

namespace StarryEye {
	class FileObject: public KObject
	{
	public:
		FileObject(const MmVirtualAddress& vaddr, uint8_t property);
		FileObject() = default;
		~FileObject() = default;

		PUNICODE_STRING FileName();

	private:
		uint8_t property_;
	};
}