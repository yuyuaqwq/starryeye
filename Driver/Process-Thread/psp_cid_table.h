#pragma once
#include "object_header.h"
#include "handle_table.h"
#include "ynstd.hpp"
#include <algorithm>


namespace StarryEye{

class PspCidTable
{
public:
	PspCidTable() = delete;

	// PspCidTable地址
	static ULONG64 Address;

	// 解密HandleTable中Handle项的路径
	static ULONG64 DecryptHandleAddress(ULONG64 addr);
	// 根据索引获取Handle对象
	static ObjectHeader GetHandleObject(ULONG64 index);

	// 获取一级TableCode下所有Handle对象
	template<class CallBackT>
	static void ForeachAllHandleObjectsInLv1TableCode(PULONG64 table, CallBackT callback)
	{
		ObjectHeader temp = nullptr;
		for (size_t i = 0; i < 512; i++)
		{
			temp = GetHandleObjectInLv1TableCode(table, i);
			if (temp)
				callback(temp);
		}
	}
	// 获取二级TableCode下所有Handle对象
	template<class CallBackT>
	static void ForeachAllHandleObjectsInLv2TableCode(PULONG64 table, CallBackT callback)
	{
		for (size_t i = 0; i < 512; i++)
		{
			if (MmIsAddressValid(table + i))
				ForeachAllHandleObjectsInLv1TableCode((PULONG64)table[i], callback);
		}
	}
	// 获取三级TableCode下所有Handle对象
	template<class CallBackT>
	static void ForeachAllHandleObjectsInLv3TableCode(PULONG64 table, CallBackT callback)
	{
		for (size_t i = 0; i < 512; i++)
		{
			if (MmIsAddressValid(table + i))
				ForeachAllHandleObjectsInLv2TableCode((PULONG64)table[i], callback);
		}
	}

	// 获取一级TableCode下指定索引的Handle对象
	static ObjectHeader GetHandleObjectInLv1TableCode(PULONG64 table, USHORT index);
	// 获取二级TableCode下指定索引的Handle对象
	static ObjectHeader GetHandleObjectInLv2TableCode(PULONG64 table, USHORT index_lv2, USHORT index_lv1);
	// 获取三级TableCode下指定索引的Handle对象
	static ObjectHeader GetHandleObjectInLv2TableCode(PULONG64 table, USHORT index_lv3, USHORT index_lv2, USHORT index_lv1);

	// 根据静态PspCidTable地址, 自动遍历所有Handle, 并区分进程和线程
	template<class ProcHandleCallBack, class ThdHandleCallBack>
	static void AutoForeachAllHandleObjects(ProcHandleCallBack proc_handle_callback, ThdHandleCallBack thd_handle_callback)
	{
		auto foreach_all_handle = [&](ObjectHeader& obj)
		{
			// TODO
		};
		HandleTable pspcidtable{ Address };
		PULONG64 table = (PULONG64)pspcidtable.TableAddress();
		switch (pspcidtable.TableLevel())
		{
		case 0:
			ForeachAllHandleObjectsInLv1TableCode(table, foreach_all_handle);
			break;
		case 1:
			ForeachAllHandleObjectsInLv2TableCode(table, foreach_all_handle);
			break;
		case 2:
			ForeachAllHandleObjectsInLv3TableCode(table, foreach_all_handle);
			break;
		default:
			break;
		}
	}
};

}
