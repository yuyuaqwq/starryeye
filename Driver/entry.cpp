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
		KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "–∂‘ÿ!\n"));
	};
	
	GlobalInit();

	DbgBreakPoint();
	try
	{
		auto table = HandleTable(HandleTable::PspCidTable.ValU64());
		auto res = HandleUtils::FindProcessInHandleTable(table, "Test.exe");
		if (!res.empty()) {
			for (auto& node : res[0].VadRoot()) {
				auto mmvad_short = node.Impl<MmVadShort>();
				DebugPrintf("Start: %llx - End: %llx\n", mmvad_short.StartingAddress(), mmvad_short.EndingAddress());
			}
			std::is_convertible_v<EProcess*, KObject*>;
		}
	}
	catch (const std::exception& ex)
	{
		DebugPrintf("≥ˆœ÷¥ÌŒÛ: %s\n", ex.what());
		return STATUS_UNSUCCESSFUL;
	}

	return STATUS_SUCCESS;
}
