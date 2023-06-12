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

	KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "ж��!\n"));
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
	DbgBreakPoint();

	HandleTable table{(ULONG64)HandleTable::PspCidTable};

	for (size_t i = 0; i < table.MaxTableSize(); i++)
	{
		auto item = table.GetHandleObject(i);
		if (item.IsVaild() && item.Type().IsProcess())
		{
			auto eproc = item.BodyObject<EProcess>();
			KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "Process Name: %s, Address: 0x%llx\n", eproc.ImageFileName(), eproc.Address()));
		}
	}

	return STATUS_SUCCESS;
}
