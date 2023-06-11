#include "eprocess.h"

using namespace StarryEye;

void StarryEye::EProcess::Init()
{
	ImageFileNameOffset = 0x5A8;
	ObjectTableOffset = 0x570;
	ThreadListHeadOffset = 0x5e0;
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

HandleTable StarryEye::EProcess::ObjectTable()
{
	return HandleTable(address_ + ObjectTableOffset);
}

KObjListEntry<EThread> StarryEye::EProcess::ThreadListHead()
{
	return KObjListEntry<EThread>(address_ + ThreadListHeadOffset, EThread::GetThreadListEntryOffset());
}
