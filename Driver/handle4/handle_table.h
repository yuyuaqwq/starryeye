#pragma once
#include <krnlib/functional.hpp>
#include <krnlib/stl_container.hpp>
#include "config/base.h"
#include "handle/object_header.h"

namespace StarryEye {

class HandleTable: public KObjectBase
{
public:
	using ForeachHandleObjectsCallBack = const krnlib::function<bool(ObjectHeader&)>&;

	inline static PVOID PspCidTable;
	static void Init();

	HandleTable(std::nullptr_t);
	HandleTable(uint64_t address);
	~HandleTable();

	// ����HandleTable��Handle���·��
	static uint64_t DecryptHandleAddress(uint64_t addr);


	// ��ȡTableCode
	uint64_t TableCode();
	// ��ȡTableCode�ȼ�
	uint8_t TableLevel();
	// ��ȡTableCode������ַ(���2λ����)
	uint64_t TableAddress();
	// TableCode����С
	uint64_t MaxTableSize();

	// ����������ȡHandle����
	ObjectHeader GetHandleObject(uint64_t index);

	// �Զ�����TableCode�ȼ���������Handle
	bool AutoForeachAllHandleObjects(ForeachHandleObjectsCallBack callback);

	// ��ȡ����Handle����(���ܲ�, ���Ƽ�ʹ��!!!)
	krnlib::list<ObjectHeader> GetAllHandleObjects();

private:
	// ��ȡһ��TableCode������Handle����
	static bool ForeachAllHandleObjectsInLv1TableCode(uint64_t* table, ForeachHandleObjectsCallBack callback);
	// ��ȡ����TableCode������Handle����
	static bool ForeachAllHandleObjectsInLv2TableCode(uint64_t* table, ForeachHandleObjectsCallBack callback);
	// ��ȡ����TableCode������Handle����
	static bool ForeachAllHandleObjectsInLv3TableCode(uint64_t* table, ForeachHandleObjectsCallBack callback);

	// ��ȡһ��TableCode��ָ��������Handle����
	static ObjectHeader GetHandleObjectInLv1TableCode(uint64_t* table, uint64_t index);
	// ��ȡ����TableCode��ָ��������Handle����
	static ObjectHeader GetHandleObjectInLv2TableCode(uint64_t* table, uint64_t index_lv2, uint64_t index_lv1);
	// ��ȡ����TableCode��ָ��������Handle����
	static ObjectHeader GetHandleObjectInLv3TableCode(uint64_t* table, uint64_t index_lv3, uint64_t index_lv2, uint64_t index_lv1);

	inline static uint64_t TableCodeOffset;
};

}