#pragma once
#include "basic/structs.h"

namespace stareye {
	class FileObject: public KObject
	{
		STAREYE_USING_BASE(KObject)
	public:
		PUNICODE_STRING FileName();
	};
}