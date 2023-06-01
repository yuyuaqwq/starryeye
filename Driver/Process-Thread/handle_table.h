#pragma once
#include <ntifs.h>

namespace StarryEye{

class HandleTable
{
public:
	HandleTable(std::nullptr_t);
	HandleTable(ULONG64 address);
	~HandleTable();

	// 获取TableCode
	ULONG64 TableCode();
	// 获取TableCode等级
	UCHAR TableLevel();
	// 获取Table地址(最后2位清零)
	ULONG64 TableAddress();

	bool IsVaild();

	operator bool();

private:
	// 地址
	ULONG64 address_;
	//TODO TableCode偏移
	UINT16 table_code_offset_;
};

}