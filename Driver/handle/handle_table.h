#pragma once
#include "basic/structs.h"
#include "object_header.h"

namespace stareye {
namespace details {
class HandleTableIterator
{
public:
	HandleTableIterator(HandleTable* table, uint64_t idx);
	~HandleTableIterator() = default;

	ObjectHeader& operator*();
	ObjectHeader* operator->();
	HandleTableIterator& operator++();
	HandleTableIterator& operator--();
	HandleTableIterator operator++(int);
	HandleTableIterator operator--(int);
	bool operator==(const HandleTableIterator& x);
	bool operator!=(const HandleTableIterator& x);

private:
	uint64_t cur_idx_;
	HandleTable* table_;
	ObjectHeader cur_obj_;
};
}

class HandleTable: public KObject
{
public:
	using ForeachHandleObjectsCallBack = std::function<bool(const ObjectHeader&)>;
	using iterator = details::HandleTableIterator;

	inline static MmVirtualAddress PspCidTable;
	static void Init();

	HandleTable() = default;
	HandleTable(const MmVirtualAddress& vaddr);
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

	iterator begin();
	iterator end();

	// �Զ�����TableCode�ȼ���������Handle
	bool AutoForeachAllHandleObjects(const ForeachHandleObjectsCallBack& callback) const;

private:
	// ����HandleTable��Handle���·��
	static uint64_t DecryptHandleAddress(uint64_t addr);

	// ��ȡһ��TableCode������Handle����
	static bool ForeachAllHandleObjectsInLv1TableCode(uint64_t* table, const ForeachHandleObjectsCallBack& callback);
	// ��ȡ����TableCode������Handle����
	static bool ForeachAllHandleObjectsInLv2TableCode(uint64_t* table, const ForeachHandleObjectsCallBack& callback);
	// ��ȡ����TableCode������Handle����
	static bool ForeachAllHandleObjectsInLv3TableCode(uint64_t* table, const ForeachHandleObjectsCallBack& callback);

	// ��ȡһ��TableCode��ָ��������Handle����
	static std::optional<ObjectHeader> GetHandleObjectInLv1TableCode(uint64_t* table, uint64_t index);
	// ��ȡ����TableCode��ָ��������Handle����
	static std::optional<ObjectHeader> GetHandleObjectInLv2TableCode(uint64_t* table, uint64_t index_lv2, uint64_t index_lv1);
	// ��ȡ����TableCode��ָ��������Handle����
	static std::optional<ObjectHeader> GetHandleObjectInLv3TableCode(uint64_t* table, uint64_t index_lv3, uint64_t index_lv2, uint64_t index_lv1);

	inline static uint64_t TableCodeOffset;
};

}