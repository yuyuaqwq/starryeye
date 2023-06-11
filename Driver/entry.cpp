#include <ntifs.h>
#include "Config/ynstd.h"

#include "Handle/handle_table.h"
#include "Handle/object_header.h"
#include "Handle/object_type.h"

#include "Process/eprocess.h"
#include "Process/kprocess.h"

#include "Thread/ethread.h"
#include "Thread/kthread.h"

#define _SCN StarryEye::ynstd::
//#include <yuJson/json.hpp>
using namespace StarryEye;

void DriverUnload(PDRIVER_OBJECT pDriverObject)
{
	UNREFERENCED_PARAMETER(pDriverObject);

	KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "п╤ть!\n"));
	return;
}

void InitOffsets()
{
	HandleTable::Init();
	ObjectHeader::Init();
	ObjectType::Init();

	EProcess::Init();
	KProcess::Init();

	EThread::Init();
	KThread::Init();
}

extern "C" NTSTATUS DriverEntry(IN PDRIVER_OBJECT pDriverObject, IN PUNICODE_STRING pRegistryPath) {
	UNREFERENCED_PARAMETER(pRegistryPath);

	pDriverObject->DriverUnload = DriverUnload;

	InitOffsets();

	_SCN string str = "??";

	//yuJson::Json json = { "fake", 666, "emm", true };


	HandleTable table{(ULONG64)HandleTable::PspCidTable};

	table.AutoForeachAllHandleObjects([&](ObjectHeader obj){
		if (obj.Type().IsProcess())
		{
			auto eproc = obj.BodyObject<EProcess>();
			KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "Name:%s Object Address: 0x%llx\n", eproc.ImageFileName(), eproc.Address()));
		}
	});

	return STATUS_SUCCESS;
}
