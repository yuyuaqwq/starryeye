#include <ntifs.h>
#include <stdexcept>
#include "Config/algorithm.h"

#include "Handle/handle_table.h"
#include "Handle/object_header.h"
#include "Handle/object_type.h"

#include "Process/eprocess.h"
#include "Process/kprocess.h"
#include "Process/vadtree.h"

#include "Thread/ethread.h"
#include "Thread/kthread.h"

//#define _SCN krnlib::
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
	table.AutoForeachAllHandleObjects([&](ObjectHeader obj) {
		if (auto res = obj.ConvToEProc(); res.Ok()) {
			auto eproc = res.OkVal();
			auto ddd = eproc.ImageFileName();
			if (eproc.CompareFileName("Everything.exe")) {
				eproc.VadRoot().Foreach([&](MmVadShort& mmvad_short) {
					if (auto res = mmvad_short->ConvToMmVad(); res.Ok()) {
						auto mmvad = res.OkVal();
						if (auto file_obj = mmvad->Subsection().ControlArea().FilePointer(); file_obj.IsVaild())
							AutoPrint("File Name: %wZ\n", file_obj.FileName());
					}
					return true;
				});
				return false;
			}
		}
		return true;
	});
	return STATUS_SUCCESS;
}
