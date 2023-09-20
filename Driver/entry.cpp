#include <ntifs.h>
#include <stdexcept>

#include "basic/init.hpp"
#include "handle/init.hpp"
#include "memory/init.hpp"
#include "process/init.hpp"
#include "thread/init.hpp"

#include "handle/utils.h"

#include <io/control.h>
#include <iostream>

//#define _SCN krnlib::
//#include <yuJson/json.hpp>
using namespace stareye;

//io::Device* g_device;
//io::Control* g_control;


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
	pDriverObject->DriverUnload = [](PDRIVER_OBJECT) {
		KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "ж��!\n"));
	};
	
	GlobalInit();

	DbgBreakPoint();
	try
	{
		auto table = HandleTable(HandleTable::PspCidTable.ValU64());
		auto res = HandleUtils::FindProcessInHandleTable(table, "Test.exe");
		if (!res.empty()) {
			auto dest = MmVirtualAddress(0x7ff7b9c8ac38, res[0]);
			dest.SetProtection(kPageExecutable | kPageWriteable);
			auto b = dest.Protection();
		}
	}
	catch (const std::exception& ex)
	{
		DebugPrintf("���ִ���: %s\n", ex.what());
		return STATUS_UNSUCCESSFUL;
	}

	return STATUS_SUCCESS;
}
