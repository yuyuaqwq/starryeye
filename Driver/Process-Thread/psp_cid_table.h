#pragma once
#include "object_header.h"
#include "handle_table.h"
#include "ynstd.hpp"


namespace StarryEye{

class PspCidTable
{
public:
	PspCidTable() = delete;

	// PspCidTable对象
	static HandleTable Object;

	// 解密HandleTable中Handle项的路径
	static ULONG64 DecrypteHandleAddress(ULONG64 addr);
	// 根据索引获取Handle对象
	static ObjectHeader GetHandleObject(ULONG64 index);

	// 获取一级TableCode下所有Handle对象
	static ynstd::list<ObjectHeader> GetAllHandleObjectsInLv1TableCode(ULONG64 table_addr);
	// 获取二级TableCode下所有Handle对象
	static ynstd::list<ObjectHeader> GetAllHandleObjectsInLv2TableCode(ULONG64 table_addr);
	// 获取三级TableCode下所有Handle对象
	static ynstd::list<ObjectHeader> GetAllHandleObjectsInLv3TableCode(ULONG64 table_addr);

	// 获取一级TableCode下指定索引的Handle对象
	static ObjectHeader GetHandleObjectInLv1TableCode(ULONG64 table_addr, USHORT index);
	// 获取二级TableCode下指定索引的Handle对象
	static ObjectHeader GetHandleObjectInLv2TableCode(ULONG64 table_addr, USHORT index_lv2, USHORT index_lv1);
	// 获取三级TableCode下指定索引的Handle对象
	static ObjectHeader GetHandleObjectInLv2TableCode(ULONG64 table_addr, USHORT index_lv3, USHORT index_lv2, USHORT index_lv1);
};

}