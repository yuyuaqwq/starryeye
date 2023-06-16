#include "handle_table.h"

using namespace StarryEye;

ULONG64 StarryEye::HandleTable::DecryptHandleAddress(ULONG64 addr)
{
	return ((LONG64)addr >> 0x10) & ~0xF;
}

void StarryEye::HandleTable::ForeachAllHandleObjectsInLv1TableCode(PULONG64 table, ForeachHandleObjectsCallBack callback)
{
	ObjectHeader temp = nullptr;
	for (SHORT i = 0; i < 512; i++)
	{
		temp = GetHandleObjectInLv1TableCode(table, i);
		if (temp.IsVaild())
			callback(temp);
	}
}

void StarryEye::HandleTable::ForeachAllHandleObjectsInLv2TableCode(PULONG64 table, ForeachHandleObjectsCallBack callback)
{
	for (size_t i = 0; i < 512; i++)
	{
		if (MmIsAddressValid(table + i))
			HandleTable::ForeachAllHandleObjectsInLv1TableCode((PULONG64)table[i], callback);
	}
}

void StarryEye::HandleTable::ForeachAllHandleObjectsInLv3TableCode(PULONG64 table, ForeachHandleObjectsCallBack callback)
{
	for (size_t i = 0; i < 512; i++)
	{
		if (MmIsAddressValid(table + i))
			HandleTable::ForeachAllHandleObjectsInLv2TableCode((PULONG64)table[i], callback);
	}
}

void StarryEye::HandleTable::Init()
{
	TableCodeOffset = 0x8;
	PspCidTable = (PVOID)0xffffcb0e0960ab40;
}

StarryEye::HandleTable::HandleTable(std::nullptr_t): KObjectBase(nullptr)
{
}

StarryEye::HandleTable::HandleTable(ULONG64 address): KObjectBase(address)
{
}

StarryEye::HandleTable::~HandleTable()
{
}

ULONG64 StarryEye::HandleTable::TableCode()
{
	return *(ULONG64*)(address_ + TableCodeOffset);
}

UCHAR StarryEye::HandleTable::TableLevel()
{
	return TableCode() & 0b11;
}

ULONG64 StarryEye::HandleTable::TableAddress()
{
	return TableCode() & ~0b11;
}

ULONG64 StarryEye::HandleTable::MaxTableSize()
{
	ULONG64 total = 1;
	for (UCHAR i = 0; i < TableLevel() + 1; i++)
		total *= 512;
	return total;
}

ObjectHeader StarryEye::HandleTable::GetHandleObject(ULONG64 index)
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

bool StarryEye::HandleTable::AutoForeachAllHandleObjects(ForeachHandleObjectsCallBack callback)
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

ynstd::list<ObjectHeader> StarryEye::HandleTable::GetAllHandleObjects()
{
	ynstd::list<ObjectHeader> total;
	AutoForeachAllHandleObjects([&](ObjectHeader& obj) {
		total.push_back(obj);
		});
	return total;
}


ObjectHeader StarryEye::HandleTable::GetHandleObjectInLv1TableCode(PULONG64 table, ULONG64 index)
{
	if (MmIsAddressValid(table) && index < 512)	// 判断目标地址是否有效
		return ObjectHeader(
			DecryptHandleAddress(table[index]) - ObjectHeader::GetBodyOffset());	// 将目标地址解密, 再将地址移动到OBJECT_HEADER头部
	return nullptr;
}

ObjectHeader StarryEye::HandleTable::GetHandleObjectInLv2TableCode(PULONG64 table, ULONG64 index_lv2, ULONG64 index_lv1)
{
	if (MmIsAddressValid(table) && index_lv2 < 512 && index_lv1 < 512)
		return GetHandleObjectInLv1TableCode((PULONG64)table[index_lv2], index_lv1);
	return nullptr;
}

ObjectHeader StarryEye::HandleTable::GetHandleObjectInLv3TableCode(PULONG64 table, ULONG64 index_lv3, ULONG64 index_lv2, ULONG64 index_lv1)
{
	if (MmIsAddressValid(table) && index_lv3 < 512 && index_lv2 < 512 && index_lv1 < 512)
		return GetHandleObjectInLv2TableCode((PULONG64)table[index_lv3], index_lv2, index_lv1);
	return nullptr;
}
