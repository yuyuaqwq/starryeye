#pragma once
#include "File/file_object.h"

namespace StarryEye {
	class ControlArea: public KObjectBase
	{
	public:
		static void Init();

		ControlArea(ULONG64 address);
		ControlArea(std::nullptr_t);
		~ControlArea();

		FileObject FilePointer();

	private:
		inline static ULONG64 FilePointerOffset;
	};
}