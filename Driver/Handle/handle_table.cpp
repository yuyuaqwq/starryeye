#include "handle_table.h"

using namespace StarryEye;

ULONG64 StarryEye::HandleTable::DecryptHandleAddress(ULONG64 addr)
{
	return ((LONG64)addr >> 0x10) & ~0xF;
}

void StarryEye::HandleTable::Init()
{
	TableCodeOffset = 0x8;	//TODO TableCode偏移
	PspCidTable = (PVOID)0xffffcb0e0960ab40;	//TODO
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

ObjectHeader StarryEye::HandleTable::GetHandleObject(ULONG64 index)
{
	// 将索引拆分
	switch (TableLevel())
	{
	case 0:
	{
		if (index > 512) return nullptr;
		return GetHandleObjectInLv1TableCode((PULONG64)TableAddress(), index);
	}
	case 1:
	{
		if (index > 512ULL * 512) return nullptr;
		return GetHandleObjectInLv2TableCode((PULONG64)TableAddress(), index / 512, index % 512);
	}
	case 2:
	{
		if (index > 512ULL * 512 * 512) return nullptr;
		return GetHandleObjectInLv3TableCode((PULONG64)TableAddress(), index / (512ULL * 512), (index % (512ULL * 512)) / 512, (index % (512ULL * 512)) % 512);
	}
	default:
		return nullptr;
	}
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
