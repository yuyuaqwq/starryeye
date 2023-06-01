#include "handle_table.h"

StarryEye::HandleTable::HandleTable(std::nullptr_t)
{
	address_ = 0;
}

StarryEye::HandleTable::HandleTable(ULONG64 address)
{
	// TODO
	address_ = address;
}

StarryEye::HandleTable::~HandleTable()
{
	// TODO
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

bool StarryEye::HandleTable::IsVaild()
{
	return MmIsAddressValid((PVOID)address_);
}

StarryEye::HandleTable::operator bool()
{
	return IsVaild();
}
