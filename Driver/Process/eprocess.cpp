#include "eprocess.h"

using namespace StarryEye;

void StarryEye::EProcess::Init()
{
	ImageFileNameOffset = 0x5A8;
	ObjectTableOffset = 0x570;
	ThreadListHeadOffset = 0x5e0;
	OwnerProcessIdOffset = 0x548;
	PriorityClassOffset = 0x5b7;
	ActiveProcessLinksOffset = 0x448;
	InheritedFromUniqueProcessIdOffset = 0x540;
	VadRootOffset = 0x7d8;
	MmVad::Init();
}

StarryEye::EProcess::EProcess(ULONG64 address): KObjectBase(address)
{
}

StarryEye::EProcess::EProcess(std::nullptr_t) : KObjectBase(nullptr)
{
}

StarryEye::EProcess::~EProcess()
{
}

PCHAR StarryEye::EProcess::ImageFileName()
{
	return (PCHAR)(address_ + ImageFileNameOffset);
}

KProcess StarryEye::EProcess::Pcb()
{
	return KProcess(address_);
}

KObjListEntry<EProcess> StarryEye::EProcess::ActiveProcessLinks()
{
	return KObjListEntry<EProcess>(address_ + ActiveProcessLinksOffset, ActiveProcessLinksOffset);
}

HandleTable StarryEye::EProcess::ObjectTable()
{
	return HandleTable(address_ + ObjectTableOffset);
}

ULONG64 StarryEye::EProcess::InheritedFromUniqueProcessId()
{
	return *(PULONG64)(address_ + InheritedFromUniqueProcessIdOffset);
}

UINT8 StarryEye::EProcess::OwnerProcessId()
{
	return *(PUINT8)(address_ + OwnerProcessIdOffset);
}

UCHAR StarryEye::EProcess::PriorityClass()
{
	return *(PUCHAR)(address_ + PriorityClassOffset);
}

KObjListEntry<EThread> StarryEye::EProcess::ThreadListHead()
{
	return KObjListEntry<EThread>(address_ + ThreadListHeadOffset, EThread::ThreadListEntryOffset);
}

VadTree StarryEye::EProcess::VadRoot()
{
	return VadTree(address_ + VadRootOffset);
}
