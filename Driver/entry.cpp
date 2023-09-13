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
	KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "Ð¶ÔØ!\n"));
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

	DbgBreakPoint();
	try
	{
		auto handle = StarryEye::HandleTable(StarryEye::HandleTable::PspCidTable.ValU64());
		handle.AutoForeachAllHandleObjects([&](const StarryEye::ObjectHeader& obj) -> bool {
			if (obj.IsProcess()) {
				auto eproc = obj.Body<EProcess>();
				DebugPrintf("%s", eproc.ImageFileName());
			}
			return true;
			});
	}
	catch (const std::exception& ex)
	{
		DebugPrintf("³öÏÖ´íÎó: %s", ex.what());
		return STATUS_UNSUCCESSFUL;
	}

	return STATUS_SUCCESS;
}
