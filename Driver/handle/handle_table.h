#pragma once
#include <krnlib/functional.hpp>
#include <krnlib/stl_container.hpp>
#include "basic/structs.h"
#include "object_header.h"

namespace stareye {

class HandleTable: public KObject
{
public:
	using ForeachHandleObjectsCallBack = krnlib::function<bool(const ObjectHeader&)>;

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
	fustd::Option<ObjectHeader> GetHandleObject(uint64_t index) const;

	// �Զ�����TableCode�ȼ���������Handle
	bool AutoForeachAllHandleObjects(const ForeachHandleObjectsCallBack& callback) const;

	// ��ȡ����Handle����(���ܲ�, ���Ƽ�ʹ��!!!)
	krnlib::list<ObjectHeader> GetAllHandleObjects() const;

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
	static fustd::Option<ObjectHeader> GetHandleObjectInLv1TableCode(uint64_t* table, uint64_t index);
	// ��ȡ����TableCode��ָ��������Handle����
	static fustd::Option<ObjectHeader> GetHandleObjectInLv2TableCode(uint64_t* table, uint64_t index_lv2, uint64_t index_lv1);
	// ��ȡ����TableCode��ָ��������Handle����
	static fustd::Option<ObjectHeader> GetHandleObjectInLv3TableCode(uint64_t* table, uint64_t index_lv3, uint64_t index_lv2, uint64_t index_lv1);

	inline static uint64_t TableCodeOffset;
};

}