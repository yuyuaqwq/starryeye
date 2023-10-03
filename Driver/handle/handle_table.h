#pragma once
#include "basic/structs.h"
#include "object_header.h"

namespace stareye {
namespace details {
class HandleTableIterator
{
public:
	HandleTableIterator(const HandleTable* table, uint16_t idx_lv3, uint16_t idx_lv2, uint16_t idx_lv1);
	~HandleTableIterator() = default;

	ObjectHeader& operator*();
	ObjectHeader* operator->();
	HandleTableIterator& operator++();
	HandleTableIterator& operator--();
	HandleTableIterator operator++(int);
	HandleTableIterator operator--(int);
	bool operator==(const HandleTableIterator& x) const;
	bool operator!=(const HandleTableIterator& x) const;

private:
	bool CheckValidIndexAndAssign();
	bool JudgeIncIdxOrInEnd();
	bool FindValidIfLv1Table();
	bool FindValidIfLv2Table();
	bool FindValidIfLv3Table();

	const HandleTable* table_;
	ObjectHeader cur_obj_;
	uint16_t idx_lv3_;
	uint16_t idx_lv2_;
	uint16_t idx_lv1_;
};
}

class HandleTable: public KObject
{
public:
	static constexpr uint16_t kMaxCount = 512;
	static constexpr uint16_t kMaxIndex = kMaxCount - 1;

	using ForeachHandleObjectsCallBack = std::function<bool(const ObjectHeader&)>;
	using iterator = details::HandleTableIterator;

	inline static MmVirtualAddress PspCidTable;
	static void Init();

	HandleTable() = default;
	HandleTable(MmVirtualAddress vaddr);
	~HandleTable() = default;

	// 获取TableCode
	uint64_t TableCode() const;

	// 获取TableCode等级
	uint8_t TableLevel() const;
	// 获取TableCode真正地址(最后2位清零)
	uint64_t TableAddress() const;
	// TableCode最大大小
	uint64_t MaxTableSize() const;

	// 根据索引获取Handle对象
	std::optional<ObjectHeader> GetHandleObject(uint64_t index) const;

	iterator begin() const;
	iterator end() const;

	std::optional<ObjectHeader> GetHandleObjectIfLv1(uint64_t index) const;
	std::optional<ObjectHeader> GetHandleObjectIfLv2(uint64_t index_lv2, uint64_t index_lv1) const;
	std::optional<ObjectHeader> GetHandleObjectIfLv3(uint64_t index_lv3, uint64_t index_lv2, uint64_t index_lv1) const;

private:
	friend class details::HandleTableIterator;

	uint64_t* TablePtr() const;

	// 解密HandleTable中Handle项的路径
	static uint64_t DecryptHandleAddress(uint64_t addr);

	// 获取一级TableCode下指定索引的Handle对象
	static std::optional<ObjectHeader> GetHandleObjectInLv1TableCode(uint64_t* table, uint64_t index);
	// 获取二级TableCode下指定索引的Handle对象
	static std::optional<ObjectHeader> GetHandleObjectInLv2TableCode(uint64_t* table, uint64_t index_lv2, uint64_t index_lv1);
	// 获取三级TableCode下指定索引的Handle对象
	static std::optional<ObjectHeader> GetHandleObjectInLv3TableCode(uint64_t* table, uint64_t index_lv3, uint64_t index_lv2, uint64_t index_lv1);

	inline static uint64_t TableCodeOffset;
};

}