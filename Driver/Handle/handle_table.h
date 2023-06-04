#pragma once
#include "Config/kobject_base.h"
#include "object_header.h"
#include "Config/global_vars.h"

namespace StarryEye {

class HandleTable: public KObjectBase
{
public:
	HandleTable(std::nullptr_t);
	HandleTable(ULONG64 address);
	~HandleTable();

	// 解密HandleTable中Handle项的路径
	static ULONG64 DecryptHandleAddress(ULONG64 addr);

	// 获取一级TableCode下所有Handle对象
	template<class CallBackT>
	static void ForeachAllHandleObjectsInLv1TableCode(PULONG64 table, CallBackT callback);
	// 获取二级TableCode下所有Handle对象
	template<class CallBackT>
	static void ForeachAllHandleObjectsInLv2TableCode(PULONG64 table, CallBackT callback);
	// 获取三级TableCode下所有Handle对象
	template<class CallBackT>
	static void ForeachAllHandleObjectsInLv3TableCode(PULONG64 table, CallBackT callback);

	// 获取一级TableCode下指定索引的Handle对象
	static ObjectHeader GetHandleObjectInLv1TableCode(PULONG64 table, USHORT index);
	// 获取二级TableCode下指定索引的Handle对象
	static ObjectHeader GetHandleObjectInLv2TableCode(PULONG64 table, USHORT index_lv2, USHORT index_lv1);
	// 获取三级TableCode下指定索引的Handle对象
	static ObjectHeader GetHandleObjectInLv3TableCode(PULONG64 table, USHORT index_lv3, USHORT index_lv2, USHORT index_lv1);

	// 获取TableCode
	ULONG64 TableCode();
	// 获取TableCode等级
	UCHAR TableLevel();
	// 获取Table地址(最后2位清零)
	ULONG64 TableAddress();

	// 根据索引获取Handle对象
	ObjectHeader GetHandleObject(ULONG64 index);

	// 自动根据TableCode等级遍历所有Handle
	template<class CallBackT>
	bool AutoForeachAllHandleObjects(CallBackT callback);

private:
	// TableCode偏移
	UINT16 table_code_offset_;
};


template<class CallBackT>
inline void HandleTable::ForeachAllHandleObjectsInLv1TableCode(PULONG64 table, CallBackT callback)
{
	ObjectHeader temp = nullptr;
	for (SHORT i = 0; i < 512; i++)
	{
		temp = GetHandleObjectInLv1TableCode(table, i);
		if (temp.IsVaild())
			callback(temp);
	}
}

template<class CallBackT>
inline void HandleTable::ForeachAllHandleObjectsInLv2TableCode(PULONG64 table, CallBackT callback)
{
	for (size_t i = 0; i < 512; i++)
	{
		if (MmIsAddressValid(table + i))
			HandleTable::ForeachAllHandleObjectsInLv1TableCode((PULONG64)table[i], callback);
	}
}

template<class CallBackT>
inline void HandleTable::ForeachAllHandleObjectsInLv3TableCode(PULONG64 table, CallBackT callback)
{
	for (size_t i = 0; i < 512; i++)
	{
		if (MmIsAddressValid(table + i))
			HandleTable::ForeachAllHandleObjectsInLv2TableCode((PULONG64)table[i], callback);
	}
}

template<class CallBackT>
inline bool HandleTable::AutoForeachAllHandleObjects(CallBackT callback)
{
	switch (TableLevel())
	{
	case 0:
		HandleTable::ForeachAllHandleObjectsInLv1TableCode((PULONG64)TableAddress(), callback);
		return true;
	case 1:
		HandleTable::ForeachAllHandleObjectsInLv2TableCode((PULONG64)TableAddress(), callback);
		return true;
	case 2:
		HandleTable::ForeachAllHandleObjectsInLv3TableCode((PULONG64)TableAddress(), callback);
		return true;
	default:
		return false;
	}
}

}