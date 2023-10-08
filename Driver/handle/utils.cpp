#include "utils.h"

namespace stareye {
	std::vector<EProcess> HandleUtils::FindProcessInHandleTable(const HandleTable& table, const char* proc_name) {
		auto same_proc_name = [&](const ObjectHeader& obj) {
			if (obj.IsProcess()) {
				auto eproc = obj.Body<EProcess>();
				return EqualString(eproc.ImageFileName(), proc_name, true);
			}
			return false;
		};
		auto to_eproc = [](const ObjectHeader& obj) {
			return obj.Body<EProcess>();
		};
		auto res = table | std::views::filter(same_proc_name) | std::views::transform(to_eproc);
		return { res.begin(), res.end() };
	}
}