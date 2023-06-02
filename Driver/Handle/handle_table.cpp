#include "handle_table.h"

ULONG64 StarryEye::HandleTable::DecryptHandleAddress(ULONG64 addr)
{
	return ((LONG64)addr >> 0x10) & ~0xF;
}

StarryEye::HandleTable::HandleTable(std::nullptr_t)
{
	address_ = 0;
}

StarryEye::HandleTable::HandleTable(ULONG64 address)
{
	address_ = address;
	table_code_offset_ = 0x8;	//TODO TableCode偏移
}

StarryEye::HandleTable::~HandleTable()
{
}

ULONG64 StarryEye::HandleTable::TableCode()
{
	return *(ULONG64*)(address_ + table_code_offset_);
}

UCHAR StarryEye::HandleTable::TableLevel()
{
	return TableCode() & 0b11;
}

ULONG64 StarryEye::HandleTable::TableAddress()
{
	return TableCode() & ~0b11;
}

StarryEye::ObjectHeader StarryEye::HandleTable::GetHandleObject(ULONG64 index)
{
	return nullptr;		// TODO
}

bool StarryEye::HandleTable::IsVaild()
{
	return MmIsAddressValid((PVOID)address_);
}


StarryEye::ObjectHeader StarryEye::HandleTable::GetHandleObjectInLv1TableCode(PULONG64 table, USHORT index)
{
	if (MmIsAddressValid(table))	// 判断目标地址是否有效
		return ObjectHeader(
			DecryptHandleAddress(table[index]) - OBJECT_HEADER_TO_BODY_SIZE);	// 将目标地址解密, 再将地址移动到OBJECT_HEADER头部
	return nullptr;
}

StarryEye::ObjectHeader StarryEye::HandleTable::GetHandleObjectInLv2TableCode(PULONG64 table, USHORT index_lv2, USHORT index_lv1)
{
	if (MmIsAddressValid(table))
		return GetHandleObjectInLv1TableCode((PULONG64)table[index_lv2], index_lv1);
	return nullptr;
}

StarryEye::ObjectHeader StarryEye::HandleTable::GetHandleObjectInLv2TableCode(PULONG64 table, USHORT index_lv3, USHORT index_lv2, USHORT index_lv1)
{
	if (MmIsAddressValid(table))
		return GetHandleObjectInLv2TableCode((PULONG64)table[index_lv3], index_lv2, index_lv1);
	return nullptr;
}
