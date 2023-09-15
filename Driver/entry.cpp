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
		auto table = HandleTable(HandleTable::PspCidTable.ValU64());
		auto res = HandleUtils::FindProcessInHandleTable(table, "Test.exe");
		if (auto opt = res[0].VadRoot().SearchNode(0x7ff6271eac38); opt.IsSome()) {
			auto node = opt.SomeVal();
			DebugPrintf("Vad Node: %llx ~ %llx", node.StartingAddress().Address(), node.EndingAddress().Address());
		}
	}
	catch (const std::exception& ex)
	{
		DebugPrintf("³öÏÖ´íÎó: %s\n", ex.what());
		return STATUS_UNSUCCESSFUL;
	}
	return STATUS_SUCCESS;
}
