#pragma once
#include <krnlib/functional.hpp>
#include <krnlib/stl_container.hpp>
#include "basic/structs.h"
#include "object_header.h"

namespace StarryEye {

class HandleTable: public KObject
{
public:
	using ForeachHandleObjectsCallBack = const krnlib::function<bool(ObjectHeader&)>&;

	inline static PVOID PspCidTable;
	static void Init();

	HandleTable() = default;
	HandleTable(const MmVirtualAddress& vaddr);
	~HandleTable() = default;

	// 获取TableCode
	uint64_t TableCode();

	// 获取TableCode等级
	uint8_t TableLevel();
	// 获取TableCode真正地址(最后2位清零)
	uint64_t TableAddress();
	// TableCode最大大小
	uint64_t MaxTableSize();

	// 根据索引获取Handle对象
	fustd::Option<ObjectHeader> GetHandleObject(uint64_t index);

	// 自动根据TableCode等级遍历所有Handle
	bool AutoForeachAllHandleObjects(ForeachHandleObjectsCallBack callback);

	// 获取所有Handle对象(性能差, 不推荐使用!!!)
	krnlib::list<ObjectHeader> GetAllHandleObjects();

private:
	// 解密HandleTable中Handle项的路径
	static uint64_t DecryptHandleAddress(uint64_t addr);

	// 获取一级TableCode下所有Handle对象
	static bool ForeachAllHandleObjectsInLv1TableCode(uint64_t* table, ForeachHandleObjectsCallBack callback);
	// 获取二级TableCode下所有Handle对象
	static bool ForeachAllHandleObjectsInLv2TableCode(uint64_t* table, ForeachHandleObjectsCallBack callback);
	// 获取三级TableCode下所有Handle对象
	static bool ForeachAllHandleObjectsInLv3TableCode(uint64_t* table, ForeachHandleObjectsCallBack callback);

	// 获取一级TableCode下指定索引的Handle对象
	static fustd::Option<ObjectHeader> GetHandleObjectInLv1TableCode(uint64_t* table, uint64_t index);
	// 获取二级TableCode下指定索引的Handle对象
	static fustd::Option<ObjectHeader> GetHandleObjectInLv2TableCode(uint64_t* table, uint64_t index_lv2, uint64_t index_lv1);
	// 获取三级TableCode下指定索引的Handle对象
	static fustd::Option<ObjectHeader> GetHandleObjectInLv3TableCode(uint64_t* table, uint64_t index_lv3, uint64_t index_lv2, uint64_t index_lv1);

	inline static uint64_t TableCodeOffset;
};

}