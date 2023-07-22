#include <ntifs.h>
#include "Config/yfstd.h"
#include "Config/algorithm.h"

#include "Handle/handle_table.h"
#include "Handle/object_header.h"
#include "Handle/object_type.h"

#include "Process/eprocess.h"
#include "Process/kprocess.h"

#include "Thread/ethread.h"
#include "Thread/kthread.h"

#define _SCN StarryEye::yfstd::
#include <yuJson/json.hpp>
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
	VadTree::Init();

	EThread::Init();
	KThread::Init();

	ControlArea::Init();
	SubSection::Init();

	AlogrithmOffsets::Init();
}

extern "C" NTSTATUS DriverEntry(IN PDRIVER_OBJECT pDriverObject, IN PUNICODE_STRING pRegistryPath) {
	UNREFERENCED_PARAMETER(pRegistryPath);

	pDriverObject->DriverUnload = DriverUnload;

	InitOffsets();

	HandleTable table{*(PULONG64)HandleTable::PspCidTable};

	DbgBreakPoint();
	for (size_t i = 0; i < table.MaxTableSize(); i++)
	{
		auto item = table.GetHandleObject(i);
		if (item.IsVaild())
		{
			KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "TypeName: %wZ\n", item.Type().Name()));
		}
		if (item.IsVaild() && item.Type().IsProcess())
		{
			auto eproc = item.BodyObject<EProcess>();

			eproc.VadRoot().Foreach([](MmVad node) {
				auto start = node->Core().StartingVpn();
				auto end = node->Core().EndingVpn();
				auto file_name = node->Subsection().ControlArea().FilePointer().FileName();
				KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "Start: %x - End: %x - Name: %wZ\n", start, end, file_name));
				});
			break;
		}
	}
	//table.AutoForeachAllHandleObjects([&](ObjectHeader obj) {
	//	if (obj.Type().IsProcess())
	//	{
	//		auto eproc = obj.BodyObject<EProcess>();
	//		KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "Process Name: %s, Address: 0x%llx\n", eproc.ImageFileName(), eproc.Address()));
	//	}
	//});
	return STATUS_SUCCESS;
}
