#pragma once
#include "object_header.h"
#include "ynstd.hpp"


namespace StarryEye{

class PspCidTable
{
public:
	PspCidTable() = delete;

	// PspCidTable地址
	static ULONG64 Address;

	// 解密HandleTable中Handle项的路径
	static ULONG64 DecrypteHandleAddress(ULONG64 addr);

	// 根据索引获取Handle对象
	static ObjectHeader GetHandleObject(ULONG64 index);

	// 获取如果是一级TableCode下所有Handle对象
	static ynstd::list<ObjectHeader> GetAllHandleObjectIfLv1TableCode();
	// 获取如果是二级TableCode下所有Handle对象
	static ynstd::list<ObjectHeader> GetAllHandleObjectIfLv2TableCode();
	// 获取如果是三级TableCode下所有Handle对象
	static ynstd::list<ObjectHeader> GetAllHandleObjectIfLv3TableCode();
};

ULONG64 PspCidTable::Address = 0x114514;

}