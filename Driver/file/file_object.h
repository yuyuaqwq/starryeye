#pragma once
#include "basic/structs.h"

namespace stareye {
	class FileObject: public KObject
	{
	public:
		using KObject::KObject;

		PUNICODE_STRING FileName();
	};
}