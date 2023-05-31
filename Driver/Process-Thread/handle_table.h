#pragma once
#include <ntddk.h>

namespace StarryEye{

class HandleTable
{
public:
	HandleTable(ULONG64 address);
	~HandleTable();

	// 获取TableCode
	ULONG64 table_code();
	// 获取TableCode等级
	UCHAR table_level();

private:
	// 地址
	ULONG64 address_;
	//TODO TableCode偏移
	ULONG32 table_code_offset_;
};

}