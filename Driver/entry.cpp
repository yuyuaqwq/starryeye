#include <ntifs.h>
#include "Process-Thread/psp_cid_table.h"
#include "ynstd.hpp"
#define _SCN StarryEye::ynstd::
//#include <yuJson/json.hpp>

ULONG64 StarryEye::PspCidTable::Address = 114514;

extern "C" NTSTATUS DriverEntry(IN PDRIVER_OBJECT pDriverObject, IN PUNICODE_STRING pRegistryPath) {

	_SCN string str = "??";

	//yuJson::Json json = { "fake", 666, "emm", true };

	KdPrint(("Enter DriverEntry!%s\n", str.c_str()));

	return STATUS_SUCCESS;
}
