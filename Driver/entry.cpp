#include <ntifs.h>
#include <stdexcept>

#include "basic/init.hpp"
#include "handle/init.hpp"
#include "memory/init.hpp"
#include "process/init.hpp"
#include "thread/init.hpp"

#include <io/control.h>

//#define _SCN krnlib::
//#include <yuJson/json.hpp>
using namespace StarryEye;

io::Device* g_device;
io::Control* g_control;

void DriverUnload(PDRIVER_OBJECT pDriverObject)
{
	UNREFERENCED_PARAMETER(pDriverObject);
	KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "п╤ть!\n"));
	return;
}

void GlobalInit()
{
	BasicInit();
	HandleInit();
	MemoryInit();
	ProcessInit();
	ThreadInit();
}

extern "C" NTSTATUS DriverEntry(IN PDRIVER_OBJECT pDriverObject, IN PUNICODE_STRING pRegistryPath) {
	UNREFERENCED_PARAMETER(pRegistryPath);
	pDriverObject->DriverUnload = DriverUnload;
	
	GlobalInit();

	auto a = StarryEye::EProcess(0xffff80888294a2c0).ImageFileName();
	DebugPrintf("%s\n", a);
	
	return STATUS_SUCCESS;
}
