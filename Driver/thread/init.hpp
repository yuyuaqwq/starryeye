#pragma once
#include "ethread.h"
#include "kthread.h"

namespace stareye {
void ThreadInit() {
	EThread::Init();
	KThread::Init();
}
}