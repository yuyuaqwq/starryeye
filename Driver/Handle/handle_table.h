#pragma once
#include "Config/base.h"
#include "Config/yfstd.h"
#include "Handle/object_header.h"

namespace StarryEye {

class HandleTable: public KObjectBase
{
public:
	using ForeachHandleObjectsCallBack = const yfstd::function<void(ObjectHeader&)>&;

	inline static PVOID PspCidTable;
	static void Init();

	HandleTable(std::nullptr_t);
	HandleTable(ULONG64 address);
	~HandleTable();

	// 解密HandleTable中Handle项的路径
	static ULONG64 DecryptHandleAddress(ULONG64 addr);

	// 获取一级TableCode下所有Handle对象
	static void ForeachAllHandleObjectsInLv1TableCode(PULONG64 table, ForeachHandleObjectsCallBack callback);
	// 获取二级TableCode下所有Handle对象
	static void ForeachAllHandleObjectsInLv2TableCode(PULONG64 table, ForeachHandleObjectsCallBack callback);
	// 获取三级TableCode下所有Handle对象
	static void ForeachAllHandleObjectsInLv3TableCode(PULONG64 table, ForeachHandleObjectsCallBack callback);

	// 获取一级TableCode下指定索引的Handle对象
	static ObjectHeader GetHandleObjectInLv1TableCode(PULONG64 table, ULONG64 index);
	// 获取二级TableCode下指定索引的Handle对象
	static ObjectHeader GetHandleObjectInLv2TableCode(PULONG64 table, ULONG64 index_lv2, ULONG64 index_lv1);
	// 获取三级TableCode下指定索引的Handle对象
	static ObjectHeader GetHandleObjectInLv3TableCode(PULONG64 table, ULONG64 index_lv3, ULONG64 index_lv2, ULONG64 index_lv1);

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
	yfstd::list<ObjectHeader> GetAllHandleObjects();

private:
	inline static ULONG64 TableCodeOffset;
};

}