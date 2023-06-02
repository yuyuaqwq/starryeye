#pragma once
#include <ntifs.h>

namespace StarryEye {

	static UNICODE_STRING ProcessType;
	static UNICODE_STRING ThreadType;

	static PVOID PspCidTable;
	static ULONG64 ObHeaderCookie;
	static PULONG64 ObTypeIndexTable;

	static void InitGlobalVars()
	{
		RtlInitUnicodeString(&ProcessType, L"Process");
		RtlInitUnicodeString(&ThreadType, L"Thread");
		PspCidTable = (PVOID)0xffffcb0e0960ab40;	//TODO
		ObTypeIndexTable = (PULONG64)0xfffff8066f911e80;
		ObHeaderCookie = 0x0000000043cc20d3;
	}
}