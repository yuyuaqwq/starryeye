#include "eprocess.h"
#include "process/vadtree.h"

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
}

EProcess::EProcess(uint64_t address) : KObjectBase(address) {}

PCHAR EProcess::ImageFileName()
{
	return (PCHAR)(address_ + ImageFileNameOffset);
}

KProcess EProcess::Pcb()
{
	return KProcess(address_);
}

ListEntry EProcess::ActiveProcessLinks()
{
	return ListEntry(address_ + ActiveProcessLinksOffset, ActiveProcessLinksOffset);
}

HandleTable EProcess::ObjectTable()
{
	return HandleTable(address_ + ObjectTableOffset);
}

uint64_t EProcess::InheritedFromUniqueProcessId()
{
	return *(uint64_t*)(address_ + InheritedFromUniqueProcessIdOffset);
}

UINT8 EProcess::OwnerProcessId()
{
	return *(PUINT8)(address_ + OwnerProcessIdOffset);
}

uint8_t EProcess::PriorityClass()
{
	return *(PUCHAR)(address_ + PriorityClassOffset);
}

ListEntry EProcess::ThreadListHead()
{
	return ListEntry(address_ + ThreadListHeadOffset, EThread::ThreadListEntryOffset);
}

VadTree EProcess::VadRoot()
{
	return VadTree(address_ + VadRootOffset);
}
bool EProcess::CompareFileName(PCCHAR file_name)
{
	return strcmp(file_name, ImageFileName()) == 0;
}
}