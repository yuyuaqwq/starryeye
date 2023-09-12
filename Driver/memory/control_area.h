#pragma once
#include "file/file_object.h"

namespace StarryEye {
	class ControlArea: public KObject
	{
	public:
		static void Init();

		ControlArea(const MmVirtualAddress& vaddr);
		ControlArea() = default;
		~ControlArea() = default;

		ExFastRef<FileObject> FilePointer();

	private:
		inline static uint64_t FilePointerOffset;
	};
}