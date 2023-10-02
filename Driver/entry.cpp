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

#include <functional>

#include <krnlib/string.hpp>

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
		//auto res = HandleUtils::FindProcessInHandleTable(table, "Everything.exe");
		//if (!res.empty()) {
		//	auto tree = res[0].VadRoot();
		//	for (auto& node : tree) {
		//		auto mmvad_short = node.Impl<MmVadShort>();
		//		krnlib::Print("Start: 0x{:016x} - End: 0x{:016x}\n", mmvad_short.StartingAddress(), mmvad_short.EndingAddress());
		//	}
		//}
		for (auto& obj : table) {
			krnlib::Print("{}\n", *obj.Type().Name());
		}
	}
	catch (const std::exception& ex)
	{
		krnlib::Print("≥ˆœ÷¥ÌŒÛ: {}\n", ex.what());
		return STATUS_UNSUCCESSFUL;
	}

	return STATUS_SUCCESS;
}
