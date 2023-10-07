#include "utils.h"

namespace stareye {

	std::vector<EProcess> HandleUtils::FindProcessInHandleTable(const HandleTable& table, const char* proc_name) {
		auto flt = [&](const ObjectHeader& obj) {
			if (obj.IsProcess()) {
				auto eproc = obj.Body<EProcess>();
				return EqualString(eproc.ImageFileName(), proc_name, true);
			}
		};
		//TODO rangeø‚÷ß≥÷
	}
}