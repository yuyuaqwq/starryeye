#include "eprocess.h"
#include "thread/ethread.h"

namespace stareye {
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

EProcess::EProcess(const MmVirtualAddress& vaddr) : KObject(vaddr) {}

char* EProcess::ImageFileName()
{
	return (vaddr_ + ImageFileNameOffset).Pointer();
}

KProcess EProcess::Pcb()
{
	return vaddr_;
}

ListEntry<EProcess> EProcess::ActiveProcessLinks()
{
	return ListEntry<EProcess>(vaddr_ + ActiveProcessLinksOffset, ActiveProcessLinksOffset);
}

HandleTable EProcess::ObjectTable()
{
	return (vaddr_ + ObjectTableOffset).ValU64();
}

uint64_t EProcess::InheritedFromUniqueProcessId()
{
	return (vaddr_ + InheritedFromUniqueProcessIdOffset).ValU64();
}

uint8_t EProcess::OwnerProcessId()
{
	return (vaddr_ + OwnerProcessIdOffset).ValU8();
}

uint8_t EProcess::PriorityClass()
{
	return (vaddr_ + PriorityClassOffset).ValU8();
}

ListEntry<EThread> EProcess::ThreadListHead()
{
	return ListEntry<EThread>(vaddr_ + ThreadListHeadOffset, EThread::ThreadListEntryOffset);
}

MmVadTree EProcess::VadRoot()
{
	return vaddr_ + VadRootOffset;
}
bool EProcess::CompareFileName(PCCHAR file_name)
{
	return strcmp(file_name, ImageFileName()) == 0;
}
}