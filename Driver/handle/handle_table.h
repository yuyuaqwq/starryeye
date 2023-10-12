#pragma once
#include "basic/structs.h"
#include "object_header.h"

namespace stareye {
namespace details {
class HandleTableConstIterator {
public:
	using iterator_category = std::bidirectional_iterator_tag;

	using value_type = ObjectHeader;
	using difference_type = ptrdiff_t;
	using pointer = const value_type*;
	using reference = const value_type&;

	HandleTableConstIterator(const HandleTable* table, int idx_lv3, int idx_lv2, int idx_lv1);
	HandleTableConstIterator();
	~HandleTableConstIterator() = default;

	reference operator*() const noexcept;
	pointer operator->() const noexcept;
	HandleTableConstIterator& operator++() noexcept;
	HandleTableConstIterator& operator--() noexcept;
	HandleTableConstIterator operator++(int) noexcept;
	HandleTableConstIterator operator--(int) noexcept;
	bool operator==(const HandleTableConstIterator& x) const noexcept;

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


class HandleTableIterator : public HandleTableConstIterator
{
	STAREYE_USING_BASE(HandleTableConstIterator)
public:
	using iterator_category = std::bidirectional_iterator_tag;
	using value_type = ObjectHeader;
	using difference_type = ptrdiff_t;
	using pointer = value_type*;
	using reference = value_type&;

	reference operator*() const noexcept;
	pointer operator->() const noexcept;

	HandleTableIterator& operator++() noexcept;
	HandleTableIterator& operator--() noexcept;
	HandleTableIterator operator++(int) noexcept;
	HandleTableIterator operator--(int) noexcept;

	bool operator==(const HandleTableIterator& x) const noexcept;
};
}

class HandleTable: public KObject
{
	STAREYE_USING_BASE(KObject)
public:
	using value_type = ObjectHeader;
	using iterator = details::HandleTableIterator;
	using const_iterator = details::HandleTableConstIterator;

	static constexpr uint16_t kMaxCount = 512;
	static constexpr uint16_t kMaxIndex = kMaxCount - 1;

	using ForeachHandleObjectsCallBack = std::function<bool(const ObjectHeader&)>;

	inline static MmVirtualAddress PspCidTable;
	static void Init();

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
	friend class details::HandleTableConstIterator;

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