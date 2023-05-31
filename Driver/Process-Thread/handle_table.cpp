#include "handle_table.h"

StarryEye::HandleTable::HandleTable(ULONG64 address)
{
	// TODO
	address_ = address;
}

StarryEye::HandleTable::~HandleTable()
{
	// TODO
}

ULONG64 StarryEye::HandleTable::table_code()
{
	return *(ULONG64*)(address_ + table_code_offset_);
}

UCHAR StarryEye::HandleTable::table_level()
{
	return table_code() & 0b11;
}
