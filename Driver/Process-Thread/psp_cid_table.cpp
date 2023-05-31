#include "psp_cid_table.h"

using namespace StarryEye;

ULONG64 PspCidTable::DecrypteHandleAddress(ULONG64 addr)
{
	return (addr >> 0x10) & ~0xF;
}

ObjectHeader PspCidTable::GetHandleObject(ULONG64 index)
{
	// TODO
	return ObjectHeader(114514);
}

ynstd::list<ObjectHeader> PspCidTable::GetAllHandleObjectsInLv1TableCode(ULONG64 table_addr)
{
	// TODO
	ynstd::list<ObjectHeader> total;
	return total;
}

ynstd::list<ObjectHeader> PspCidTable::GetAllHandleObjectsInLv2TableCode(ULONG64 table_addr)
{
	// TODO
	ynstd::list<ObjectHeader> total;
	return total;
}

ynstd::list<ObjectHeader> PspCidTable::GetAllHandleObjectsInLv3TableCode(ULONG64 table_addr)
{
	// TODO
	ynstd::list<ObjectHeader> total;
	return total;
}

ObjectHeader StarryEye::PspCidTable::GetHandleObjectInLv1TableCode(ULONG64 table_addr, USHORT index)
{
	// TODO
	return ObjectHeader(114514);
}

ObjectHeader StarryEye::PspCidTable::GetHandleObjectInLv2TableCode(ULONG64 table_addr, USHORT index_lv2, USHORT index_lv1)
{
	// TODO
	return ObjectHeader(114514);
}

ObjectHeader StarryEye::PspCidTable::GetHandleObjectInLv2TableCode(ULONG64 table_addr, USHORT index_lv3, USHORT index_lv2, USHORT index_lv1)
{
	// TODO
	return ObjectHeader(114514);
}
