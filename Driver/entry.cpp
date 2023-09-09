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

#include "memory/init.hpp"

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

	MemoryInit();

	AlogrithmOffsets::Init();
}

extern "C" NTSTATUS DriverEntry(IN PDRIVER_OBJECT pDriverObject, IN PUNICODE_STRING pRegistryPath) {
	UNREFERENCED_PARAMETER(pRegistryPath);

	//g_device = new io::Device;
	//g_device->Create(pDriverObject, L"\\Device\\StarryEye");

	//g_control = new io::Control;
	//g_control->Create(g_device, L"\\??\\StarrtEye");

	//g_control->Register(1, [](void* buf, size_t len)->size_t {
	//	return -1;
	//	}
	//);

	pDriverObject->DriverUnload = DriverUnload;
	
	InitOffsets();
	
	return STATUS_SUCCESS;
}
