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

	// ����HandleTable��Handle���·��
	static ULONG64 DecryptHandleAddress(ULONG64 addr);


	// ��ȡTableCode
	ULONG64 TableCode();
	// ��ȡTableCode�ȼ�
	UCHAR TableLevel();
	// ��ȡTableCode������ַ(���2λ����)
	ULONG64 TableAddress();
	// TableCode����С
	ULONG64 MaxTableSize();

	// ����������ȡHandle����
	ObjectHeader GetHandleObject(ULONG64 index);

	// �Զ�����TableCode�ȼ���������Handle
	bool AutoForeachAllHandleObjects(ForeachHandleObjectsCallBack callback);

	// ��ȡ����Handle����(���ܲ�, ���Ƽ�ʹ��!!!)
	krnlib::list<ObjectHeader> GetAllHandleObjects();

private:
	// ��ȡһ��TableCode������Handle����
	static bool ForeachAllHandleObjectsInLv1TableCode(PULONG64 table, ForeachHandleObjectsCallBack callback);
	// ��ȡ����TableCode������Handle����
	static bool ForeachAllHandleObjectsInLv2TableCode(PULONG64 table, ForeachHandleObjectsCallBack callback);
	// ��ȡ����TableCode������Handle����
	static bool ForeachAllHandleObjectsInLv3TableCode(PULONG64 table, ForeachHandleObjectsCallBack callback);

	// ��ȡһ��TableCode��ָ��������Handle����
	static ObjectHeader GetHandleObjectInLv1TableCode(PULONG64 table, ULONG64 index);
	// ��ȡ����TableCode��ָ��������Handle����
	static ObjectHeader GetHandleObjectInLv2TableCode(PULONG64 table, ULONG64 index_lv2, ULONG64 index_lv1);
	// ��ȡ����TableCode��ָ��������Handle����
	static ObjectHeader GetHandleObjectInLv3TableCode(PULONG64 table, ULONG64 index_lv3, ULONG64 index_lv2, ULONG64 index_lv1);

	inline static ULONG64 TableCodeOffset;
};

}