#include "utils.h"

namespace stareye {
	std::vector<EProcess> HandleUtils::FindProcessInHandleTable(const HandleTable& table, const char* proc_name) {
		std::vector<EProcess> total;
		for (auto& obj : table) {
			if (obj.IsProcess()) {
				auto eproc = obj.Body<EProcess>();
				if (EqualString(eproc.ImageFileName(), proc_name, true)) {
					total.push_back(eproc);
				}
			}
		}
		return total;
	}
}