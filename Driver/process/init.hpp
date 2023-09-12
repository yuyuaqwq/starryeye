#pragma once
#include "eprocess.h"
#include "kprocess.h"
#include "vad_flags.h"
#include "vadtree.h"

namespace StarryEye {
void ProcessInit() {
	EProcess::Init();
	KProcess::Init();
	MmVadFlags::Init();
	MmVadTree::Init();
}
}