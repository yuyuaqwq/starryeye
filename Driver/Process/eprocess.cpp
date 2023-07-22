#include "eprocess.h"

namespace StarryEye {
void EProcess::Init()
{
	ImageFileNameOffset = 0x5A8;
	ObjectTableOffset = 0x570;
	ThreadListHeadOffset = 0x5e0;
	OwnerProcessIdOffset = 0x548;
	PriorityClassOffset = 0x5b7;
	ActiveProcessLinksOffset = 0x448;
	InheritedFromUniqueProcessIdOffset = 0x540;
	VadRootOffset = 0x7d8;
	VadTree::Init();
}

EProcess::EProcess(ULONG64 address) : KObjectBase(address) {}
EProcess::EProcess(std::nullptr_t) : KObjectBase(nullptr) {}
EProcess::~EProcess() {}

PCHAR EProcess::ImageFileName()
{
	return (PCHAR)(address_ + ImageFileNameOffset);
}

KProcess EProcess::Pcb()
{
	return KProcess(address_);
}

KObjListEntry<EProcess> EProcess::ActiveProcessLinks()
{
	return KObjListEntry<EProcess>(address_ + ActiveProcessLinksOffset, ActiveProcessLinksOffset);
}

HandleTable EProcess::ObjectTable()
{
	return HandleTable(address_ + ObjectTableOffset);
}

ULONG64 EProcess::InheritedFromUniqueProcessId()
{
	return *(PULONG64)(address_ + InheritedFromUniqueProcessIdOffset);
}

UINT8 EProcess::OwnerProcessId()
{
	return *(PUINT8)(address_ + OwnerProcessIdOffset);
}

UCHAR EProcess::PriorityClass()
{
	return *(PUCHAR)(address_ + PriorityClassOffset);
}

KObjListEntry<EThread> EProcess::ThreadListHead()
{
	return KObjListEntry<EThread>(address_ + ThreadListHeadOffset, EThread::ThreadListEntryOffset);
}

VadTree EProcess::VadRoot()
{
	return VadTree(address_ + VadRootOffset);
}
}