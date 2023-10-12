#pragma once
#include "file/file_object.h"

namespace stareye {
	class ControlArea: public KObject
	{
		STAREYE_USING_BASE(KObject)
	public:
		static void Init();

		ExFastRef<FileObject> FilePointer();

	private:
		inline static uint64_t FilePointerOffset;
	};
}