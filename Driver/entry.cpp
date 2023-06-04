#include <ntifs.h>
#include "Config/ynstd.h"
#include "Config/global_vars.h"
#include "Handle/handle_table.h"
#include "Handle/object_type.h"
#include "Process/eprocess.h"
#define _SCN StarryEye::ynstd::
//#include <yuJson/json.hpp>

void DriverUnload(PDRIVER_OBJECT pDriverObject)
{
	KdPrint(("п╤ть!\n"));
	return;
}

extern "C" NTSTATUS DriverEntry(IN PDRIVER_OBJECT pDriverObject, IN PUNICODE_STRING pRegistryPath) {

	pDriverObject->DriverUnload = DriverUnload;

	StarryEye::InitGlobalVars();

	_SCN string str = "??";

	//yuJson::Json json = { "fake", 666, "emm", true };


	StarryEye::HandleTable table{(ULONG64)StarryEye::PspCidTable};

	table.AutoForeachAllHandleObjects([&](StarryEye::ObjectHeader obj){
		if (obj.Type().IsProcess())
		{
			auto name = obj.BodyObject<StarryEye::EProcess>().ImageFileName();
			KdPrint(("%s\n", name));
		}
	});

	return STATUS_SUCCESS;
}
