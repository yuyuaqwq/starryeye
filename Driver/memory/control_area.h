#pragma once
#include "file/file_object.h"

namespace StarryEye {
	class ControlArea: public KObjectBase
	{
	public:
		static void Init();

		ControlArea(uint64_t address);
		ControlArea() = default;
		~ControlArea() = default;

		FileObject FilePointer();

	private:
		inline static uint64_t FilePointerOffset;
	};
}