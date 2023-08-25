#include "handle_table.h"

namespace StarryEye {
uint64_t HandleTable::DecryptHandleAddress(uint64_t addr)
{
	return ((LONG64)addr >> 0x10) & ~0xF;
}

bool HandleTable::ForeachAllHandleObjectsInLv1TableCode(uint64_t* table, ForeachHandleObjectsCallBack callback)
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

bool HandleTable::ForeachAllHandleObjectsInLv2TableCode(uint64_t* table, ForeachHandleObjectsCallBack callback)
{
	for (size_t i = 0; i < 512; i++) {
		if (MmIsAddressValid(table + i)) {
			if (!HandleTable::ForeachAllHandleObjectsInLv1TableCode((uint64_t*)table[i], callback)) return false;
		}
	}
	return true;
}

bool HandleTable::ForeachAllHandleObjectsInLv3TableCode(uint64_t* table, ForeachHandleObjectsCallBack callback)
{
	for (size_t i = 0; i < 512; i++) {
		if (MmIsAddressValid(table + i)) {
			if (!HandleTable::ForeachAllHandleObjectsInLv2TableCode((uint64_t*)table[i], callback)) return false;
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
HandleTable::HandleTable(uint64_t address) : KObjectBase(address) {}

uint64_t HandleTable::TableCode()
{
	return *(uint64_t*)(address_ + TableCodeOffset);
}

uint8_t HandleTable::TableLevel()
{
	return TableCode() & 0b11;
}

uint64_t HandleTable::TableAddress()
{
	return TableCode() & ~0b11;
}

uint64_t HandleTable::MaxTableSize()
{
	uint64_t total = 1;
	for (uint8_t i = 0; i < TableLevel() + 1; i++)
		total *= 512;
	return total;
}

ObjectHeader HandleTable::GetHandleObject(uint64_t index)
{
	// 将索引拆分
	if (index >= MaxTableSize()) return nullptr;

	switch (TableLevel())
	{
	case 0:
		return GetHandleObjectInLv1TableCode((uint64_t*)TableAddress(), index);
	case 1:
		return GetHandleObjectInLv2TableCode((uint64_t*)TableAddress(), index / 512, index % 512);
	case 2:
		return GetHandleObjectInLv3TableCode((uint64_t*)TableAddress(), index / (512ULL * 512), (index % (512ULL * 512)) / 512, (index % (512ULL * 512)) % 512);
	default:
		return nullptr;
	}
}

bool HandleTable::AutoForeachAllHandleObjects(ForeachHandleObjectsCallBack callback)
{
	switch (TableLevel())
	{
	case 0:
		HandleTable::ForeachAllHandleObjectsInLv1TableCode((uint64_t*)TableAddress(), callback);
		return true;
	case 1:
		HandleTable::ForeachAllHandleObjectsInLv2TableCode((uint64_t*)TableAddress(), callback);
		return true;
	case 2:
		HandleTable::ForeachAllHandleObjectsInLv3TableCode((uint64_t*)TableAddress(), callback);
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


ObjectHeader HandleTable::GetHandleObjectInLv1TableCode(uint64_t* table, uint64_t index)
{
	if (MmIsAddressValid(table) && index < 512)	// 判断目标地址是否有效
		return ObjectHeader(
			DecryptHandleAddress(table[index]) - ObjectHeader::GetBodyOffset());	// 将目标地址解密, 再将地址移动到OBJECT_HEADER头部
	return nullptr;
}

ObjectHeader HandleTable::GetHandleObjectInLv2TableCode(uint64_t* table, uint64_t index_lv2, uint64_t index_lv1)
{
	if (MmIsAddressValid(table) && index_lv2 < 512 && index_lv1 < 512)
		return GetHandleObjectInLv1TableCode((uint64_t*)table[index_lv2], index_lv1);
	return nullptr;
}

ObjectHeader HandleTable::GetHandleObjectInLv3TableCode(uint64_t* table, uint64_t index_lv3, uint64_t index_lv2, uint64_t index_lv1)
{
	if (MmIsAddressValid(table) && index_lv3 < 512 && index_lv2 < 512 && index_lv1 < 512)
		return GetHandleObjectInLv2TableCode((uint64_t*)table[index_lv3], index_lv2, index_lv1);
	return nullptr;
}
}