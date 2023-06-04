#pragma once
#include "Config/kobject_base.h"

namespace StarryEye {
	class EProcess: public KObjectBase
	{
	public:
		EProcess(ULONG64 address);
		EProcess(std::nullptr_t);
		~EProcess();

		PCHAR ImageFileName();

	private:
		UINT16 image_file_name_offset_;
	};
}