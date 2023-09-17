#pragma once
#include "handle_table.h"
#include "process/eprocess.h"

namespace stareye {
	class HandleUtils {
	public:
		static krnlib::vector<EProcess> FindProcessInHandleTable(const HandleTable& table, const char* proc_name);
	};
}