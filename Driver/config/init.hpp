#pragma once
#include "config/virtual_addr.h"

namespace StarryEye {
	void ConfigInit() {
		MmVirtualAddress::Init();
	}
}