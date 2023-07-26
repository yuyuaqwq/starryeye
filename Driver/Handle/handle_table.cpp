#include "handle_table.h"

namespace StarryEye {
ULONG64 HandleTable::DecryptHandleAddress(ULONG64 addr)
{
	return ((LONG64)addr >> 0x10) & ~0xF;
}

bool HandleTable::ForeachAllHandleObjectsInLv1TableCode(PULONG64 table, ForeachHandleObjectsCallBack callback)
{
	ObjectHeader temp = nullptr;
	for (SHORT i = 0; i < 512; i++)
	{
		temp = GetHandleObjectInLv1TableCode(table, i);
		if (temp.IsVaild()) {
			if (!callback(temp)) return false;
		}
	}
	return true;
}

bool HandleTable::ForeachAllHandleObjectsInLv2TableCode(PULONG64 table, ForeachHandleObjectsCallBack callback)
{
	for (size_t i = 0; i < 512; i++) {
		if (MmIsAddressValid(table + i)) {
			if (!HandleTable::ForeachAllHandleObjectsInLv1TableCode((PULONG64)table[i], callback)) return false;
		}
	}
	return true;
}

bool HandleTable::ForeachAllHandleObjectsInLv3TableCode(PULONG64 table, ForeachHandleObjectsCallBack callback)
{
	for (size_t i = 0; i < 512; i++) {
		if (MmIsAddressValid(table + i)) {
			if (!HandleTable::ForeachAllHandleObjectsInLv2TableCode((PULONG64)table[i], callback)) return false;
		}
	}
	return true;
}

void HandleTable::Init()
{
	TableCodeOffset = 0x8;
	PspCidTable = (PVOID)0xfffff80260efc5d0;	//TODO PspCidTable
}

HandleTable::HandleTable(std::nullptr_t) : KObjectBase(nullptr) {}
HandleTable::HandleTable(ULONG64 address) : KObjectBase(address) {}
HandleTable::~HandleTable() {}

ULONG64 HandleTable::TableCode()
{
	return *(ULONG64*)(address_ + TableCodeOffset);
}

UCHAR HandleTable::TableLevel()
{
	return TableCode() & 0b11;
}

ULONG64 HandleTable::TableAddress()
{
	return TableCode() & ~0b11;
}

ULONG64 HandleTable::MaxTableSize()
{
	ULONG64 total = 1;
	for (UCHAR i = 0; i < TableLevel() + 1; i++)
		total *= 512;
	return total;
}

ObjectHeader HandleTable::GetHandleObject(ULONG64 index)
{
	// 将索引拆分
	if (index >= MaxTableSize()) return nullptr;

	switch (TableLevel())
	{
	case 0:
		return GetHandleObjectInLv1TableCode((PULONG64)TableAddress(), index);
	case 1:
		return GetHandleObjectInLv2TableCode((PULONG64)TableAddress(), index / 512, index % 512);
	case 2:
		return GetHandleObjectInLv3TableCode((PULONG64)TableAddress(), index / (512ULL * 512), (index % (512ULL * 512)) / 512, (index % (512ULL * 512)) % 512);
	default:
		return nullptr;
	}
}

bool HandleTable::AutoForeachAllHandleObjects(ForeachHandleObjectsCallBack callback)
{
	switch (TableLevel())
	{
	case 0:
		HandleTable::ForeachAllHandleObjectsInLv1TableCode((PULONG64)TableAddress(), callback);
		return true;
	case 1:
		HandleTable::ForeachAllHandleObjectsInLv2TableCode((PULONG64)TableAddress(), callback);
		return true;
	case 2:
		HandleTable::ForeachAllHandleObjectsInLv3TableCode((PULONG64)TableAddress(), callback);
		return true;
	default:
		return false;
	}
}

krnlib::list<ObjectHeader> HandleTable::GetAllHandleObjects()
{
	krnlib::list<ObjectHeader> total;
	AutoForeachAllHandleObjects([&](ObjectHeader& obj) {
		total.push_back(obj);
		return true;
		});
	return total;
}


ObjectHeader HandleTable::GetHandleObjectInLv1TableCode(PULONG64 table, ULONG64 index)
{
	if (MmIsAddressValid(table) && index < 512)	// 判断目标地址是否有效
		return ObjectHeader(
			DecryptHandleAddress(table[index]) - ObjectHeader::GetBodyOffset());	// 将目标地址解密, 再将地址移动到OBJECT_HEADER头部
	return nullptr;
}

ObjectHeader HandleTable::GetHandleObjectInLv2TableCode(PULONG64 table, ULONG64 index_lv2, ULONG64 index_lv1)
{
	if (MmIsAddressValid(table) && index_lv2 < 512 && index_lv1 < 512)
		return GetHandleObjectInLv1TableCode((PULONG64)table[index_lv2], index_lv1);
	return nullptr;
}

ObjectHeader HandleTable::GetHandleObjectInLv3TableCode(PULONG64 table, ULONG64 index_lv3, ULONG64 index_lv2, ULONG64 index_lv1)
{
	if (MmIsAddressValid(table) && index_lv3 < 512 && index_lv2 < 512 && index_lv1 < 512)
		return GetHandleObjectInLv2TableCode((PULONG64)table[index_lv3], index_lv2, index_lv1);
	return nullptr;
}
}