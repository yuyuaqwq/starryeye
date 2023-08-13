#include <ntifs.h>
#include <stdexcept>
#include "config/algorithm.h"

#include "handle/handle_table.h"
#include "handle/object_header.h"
#include "handle/object_type.h"

#include "process/eprocess.h"
#include "process/kprocess.h"
#include "process/vadtree.h"

#include "thread/ethread.h"
#include "thread/kthread.h"

#include <io/control.h>

//#define _SCN krnlib::
//#include <yuJson/json.hpp>
using namespace StarryEye;

io::Device* g_device;
io::Control* g_control;

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

	g_device = new io::Device;
	g_device->Create(pDriverObject, L"\\Device\\StarryEye");

	g_control = new io::Control;
	g_control->Create(g_device, L"\\??\\StarrtEye");

	g_control->Register(1, [](void* buf, size_t len)->size_t {
		return -1;
		}
	);

	pDriverObject->DriverUnload = DriverUnload;

	InitOffsets();


	HandleTable table{*(uint64_t*)HandleTable::PspCidTable};
	//DbgBreakPoint();

	table.AutoForeachAllHandleObjects([&](ObjectHeader obj) {
		if (auto res = obj.ConvToEProc(); res.IsSome()) {
			auto eproc = res.SomeVal();
			//if (eproc.CompareFileName("Everything.exe")) {
			//	eproc.VadRoot().Foreach([&](MmVadShort& mmvad_short) {
			//		if (auto res = mmvad_short->ConvToMmVad(); res.IsSome()) {
			//			auto mmvad = res.SomeVal();
			//			if (auto file_obj = mmvad->Subsection().ControlArea().FilePointer(); file_obj.IsVaild())
			//				DebugPrintf("File Name: %wZ\n", file_obj.FileName());
			//		}
			//		return true;
			//	});
			//	return false;
			//}
			DebugPrintf("Proc Name: %s\n", eproc.ImageFileName());
			return true;
		}
		//return true;
	});
	return STATUS_SUCCESS;
}
