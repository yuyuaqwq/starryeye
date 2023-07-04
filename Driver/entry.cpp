#include <ntifs.h>
#include "Config/ynstd.h"
#include "Config/algorithm.h"

#include "Handle/handle_table.h"
#include "Handle/object_header.h"
#include "Handle/object_type.h"

#include "Process/eprocess.h"
#include "Process/kprocess.h"

#include "Thread/ethread.h"
#include "Thread/kthread.h"

#define _SCN StarryEye::ynstd::
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

	EThread::Init();
	KThread::Init();

	AlogrithmOffsets::Init();
}

extern "C" NTSTATUS DriverEntry(IN PDRIVER_OBJECT pDriverObject, IN PUNICODE_STRING pRegistryPath) {
	UNREFERENCED_PARAMETER(pRegistryPath);

	pDriverObject->DriverUnload = DriverUnload;

	InitOffsets();

	HandleTable table{(ULONG64)HandleTable::PspCidTable};

	for (size_t i = 0; i < table.MaxTableSize(); i++)
	{
		auto item = table.GetHandleObject(i);
		if (item.IsVaild() && item.Type().IsProcess())
		{
			auto eproc = item.BodyObject<EProcess>();

			//eproc.VadRoot().Foreach([](VadNode& node) {
			//	auto start = node->Core().StartingVpn();
			//	auto end = node->Core().EndingVpn();
			//	KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "Start: %x --- End: %x\n", start, end));
			//	});
			auto nodes = eproc.VadRoot().GetAllNodes();
			for (auto& node : nodes)
			{
				auto start = node->Core().StartingVpn();
				auto end = node->Core().EndingVpn();
				KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "Start: %x --- End: %x\n", start, end));
			}
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
	auto obj = yuJson::Json::MakeObject();
	auto str = obj.Print();
	auto& qqq = obj.GetObject().At("abc");
	obj["aa"]["bb"];
	yuJson::Json json{ "awdwd", yuJson::Json({ "aa", 123 }), "emmm", 3333};
	json["cccccc"] = "qaq";
	json["awdwd"]["aa"] = { 1,2,3,4 };
	return STATUS_SUCCESS;
}
