#pragma once
#include "ethread.h"
#include "kthread.h"

namespace StarryEye {
void ThreadInit() {
	EThread::Init();
	KThread::Init();
}
}