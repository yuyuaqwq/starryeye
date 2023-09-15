#pragma once
#include "eprocess.h"
#include "kprocess.h"
#include "vad_flags.h"
#include "vadtree.h"

namespace stareye {
void ProcessInit() {
	EProcess::Init();
	KProcess::Init();
	MmVadFlags::Init();
	MmVadTree::Init();
}
}