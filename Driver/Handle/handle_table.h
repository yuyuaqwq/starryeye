#pragma once
#include <krnlib/functional.hpp>
#include <krnlib/stl_container.hpp>
#include "Config/base.h"
#include "Handle/object_header.h"

namespace StarryEye {

class HandleTable: public KObjectBase
{
public:
	using ForeachHandleObjectsCallBack = const krnlib::function<bool(ObjectHeader&)>&;

	inline static PVOID PspCidTable;
	static void Init();

	HandleTable(std::nullptr_t);
	HandleTable(ULONG64 address);
	~HandleTable();

	// 解密HandleTable中Handle项的路径
	static ULONG64 DecryptHandleAddress(ULONG64 addr);


	// 获取TableCode
	ULONG64 TableCode();
	// 获取TableCode等级
	UCHAR TableLevel();
	// 获取TableCode真正地址(最后2位清零)
	ULONG64 TableAddress();
	// TableCode最大大小
	ULONG64 MaxTableSize();

	// 根据索引获取Handle对象
	ObjectHeader GetHandleObject(ULONG64 index);

	// 自动根据TableCode等级遍历所有Handle
	bool AutoForeachAllHandleObjects(ForeachHandleObjectsCallBack callback);

	// 获取所有Handle对象(性能差, 不推荐使用!!!)
	krnlib::list<ObjectHeader> GetAllHandleObjects();

private:
	// 获取一级TableCode下所有Handle对象
	static bool ForeachAllHandleObjectsInLv1TableCode(PULONG64 table, ForeachHandleObjectsCallBack callback);
	// 获取二级TableCode下所有Handle对象
	static bool ForeachAllHandleObjectsInLv2TableCode(PULONG64 table, ForeachHandleObjectsCallBack callback);
	// 获取三级TableCode下所有Handle对象
	static bool ForeachAllHandleObjectsInLv3TableCode(PULONG64 table, ForeachHandleObjectsCallBack callback);

	// 获取一级TableCode下指定索引的Handle对象
	static ObjectHeader GetHandleObjectInLv1TableCode(PULONG64 table, ULONG64 index);
	// 获取二级TableCode下指定索引的Handle对象
	static ObjectHeader GetHandleObjectInLv2TableCode(PULONG64 table, ULONG64 index_lv2, ULONG64 index_lv1);
	// 获取三级TableCode下指定索引的Handle对象
	static ObjectHeader GetHandleObjectInLv3TableCode(PULONG64 table, ULONG64 index_lv3, ULONG64 index_lv2, ULONG64 index_lv1);

	inline static ULONG64 TableCodeOffset;
};

}