#pragma once
#include "handle_table.h"
#include "process/eprocess.h"
#include <ranges>

namespace stareye {
	class HandleUtils {
	public:
		static std::vector<EProcess> FindProcessInHandleTable(const HandleTable& table, const char* proc_name);
	};
}