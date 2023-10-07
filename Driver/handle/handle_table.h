#pragma once
#include "basic/structs.h"
#include "object_header.h"

namespace stareye {
namespace details {
class HandleTableIterator
{
public:
	HandleTableIterator(const HandleTable* table, uint16_t idx_lv3, uint16_t idx_lv2, uint16_t idx_lv1);
	HandleTableIterator();
	~HandleTableIterator() = default;

	using value_type = HandleTable;
	using difference_type = ptrdiff_t;

	ObjectHeader& operator*() noexcept;
	ObjectHeader* operator->() noexcept;
	const ObjectHeader& operator*() const noexcept;
	const ObjectHeader* operator->() const noexcept;
	HandleTableIterator& operator++() noexcept;
	HandleTableIterator& operator--() noexcept;
	HandleTableIterator operator++(int) noexcept;
	HandleTableIterator operator--(int) noexcept;
	bool operator==(const HandleTableIterator& x) const noexcept;
	bool operator!=(const HandleTableIterator& x) const noexcept;

private:
	bool CheckValidIndexAndAssign() noexcept;
	bool JudgeIncIdxOrInEnd() noexcept;
	bool FindValidIfLv1Table() noexcept;
	bool FindValidIfLv2Table() noexcept;
	bool FindValidIfLv3Table() noexcept;

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
	using value_type = ObjectHeader;
	using difference_type = std::ptrdiff_t;
	using iterator = details::HandleTableIterator;
	using const_iterator = const iterator;

	static constexpr uint16_t kMaxCount = 512;
	static constexpr uint16_t kMaxIndex = kMaxCount - 1;

	using ForeachHandleObjectsCallBack = std::function<bool(const ObjectHeader&)>;

	inline static MmVirtualAddress PspCidTable;
	static void Init();

	HandleTable() = default;
	HandleTable(MmVirtualAddress vaddr);
	~HandleTable() = default;

	// ��ȡTableCode
	uint64_t TableCode() const;

	// ��ȡTableCode�ȼ�
	uint8_t TableLevel() const;
	// ��ȡTableCode������ַ(���2λ����)
	uint64_t TableAddress() const;
	// TableCode����С
	uint64_t MaxTableSize() const;

	// ����������ȡHandle����
	std::optional<ObjectHeader> GetHandleObject(uint64_t index) const;

	const_iterator begin() const;
	const_iterator end() const;
	iterator begin();
	iterator end();

	std::optional<ObjectHeader> GetHandleObjectIfLv1(uint64_t index) const;
	std::optional<ObjectHeader> GetHandleObjectIfLv2(uint64_t index_lv2, uint64_t index_lv1) const;
	std::optional<ObjectHeader> GetHandleObjectIfLv3(uint64_t index_lv3, uint64_t index_lv2, uint64_t index_lv1) const;

private:
	friend class details::HandleTableIterator;

	uint64_t* TablePtr() const;
	std::tuple<uint16_t, uint16_t, uint16_t> GetBeginIdx() const;

	// ����HandleTable��Handle���·��
	static uint64_t DecryptHandleAddress(uint64_t addr);

	// ��ȡһ��TableCode��ָ��������Handle����
	static std::optional<ObjectHeader> GetHandleObjectInLv1TableCode(uint64_t* table, uint64_t index);
	// ��ȡ����TableCode��ָ��������Handle����
	static std::optional<ObjectHeader> GetHandleObjectInLv2TableCode(uint64_t* table, uint64_t index_lv2, uint64_t index_lv1);
	// ��ȡ����TableCode��ָ��������Handle����
	static std::optional<ObjectHeader> GetHandleObjectInLv3TableCode(uint64_t* table, uint64_t index_lv3, uint64_t index_lv2, uint64_t index_lv1);

	inline static uint64_t TableCodeOffset;
};

}