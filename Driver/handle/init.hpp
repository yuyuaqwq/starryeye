#pragma once
#include "handle_table.h"
#include "object_header.h"
#include "object_type.h"

namespace stareye {
void HandleInit() {
	HandleTable::Init();
	ObjectHeader::Init();
	ObjectType::Init();
}
}