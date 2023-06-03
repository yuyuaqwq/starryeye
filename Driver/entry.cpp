#include <ntifs.h>
#include "ynstd.hpp"
#include "global_vars.hpp"
#include "Handle/handle_table.h"
#include "Handle/object_type.h"
#define _SCN StarryEye::ynstd::
//#include <yuJson/json.hpp>

void DriverUnload(PDRIVER_OBJECT pDriverObject)
{
	KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "п╤ть!\n"));
	return;
}

extern "C" NTSTATUS DriverEntry(IN PDRIVER_OBJECT pDriverObject, IN PUNICODE_STRING pRegistryPath) {

	pDriverObject->DriverUnload = DriverUnload;

	DbgBreakPoint();

	StarryEye::InitGlobalVars();

	_SCN string str = "??";

	//yuJson::Json json = { "fake", 666, "emm", true };

	StarryEye::HandleTable table{(ULONG64)StarryEye::PspCidTable};

	table.AutoForeachAllHandleObjects([&](StarryEye::ObjectHeader obj) {
		KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "HandleType: %wZ", obj.Type().Name()));
		});

	return STATUS_SUCCESS;
}
