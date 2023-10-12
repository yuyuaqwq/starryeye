#include "structs.h"

namespace stareye {
KObject::KObject(MmVirtualAddress vaddr) noexcept: vaddr_(vaddr) {}
MmVirtualAddress KObject::VAddr() const noexcept {
	return vaddr_;
}
}