#pragma once
#include "Config/base.h"
#include "Config/algorithm.h"
#include "Process/kprocess.h"
#include "Process/vadtree.h"
#include "Thread/ethread.h"
#include "Handle/handle_table.h"

namespace StarryEye {
class EThread;

class EProcess: public KObjectBase
{
public:
	static void Init();

	EProcess(ULONG64 address);
	EProcess(std::nullptr_t);
	~EProcess();

	PCHAR ImageFileName();
	KProcess Pcb();
	KObjListEntry<EProcess> ActiveProcessLinks();
	HandleTable ObjectTable();
	ULONG64 InheritedFromUniqueProcessId();
	UINT8 OwnerProcessId();
	UCHAR PriorityClass();
	KObjListEntry<EThread> ThreadListHead();
	VadTree VadRoot();

private:
	inline static ULONG64 ImageFileNameOffset;
	inline static ULONG64 ObjectTableOffset;
	inline static ULONG64 ThreadListHeadOffset;
	inline static ULONG64 OwnerProcessIdOffset;
	inline static ULONG64 PriorityClassOffset;
	inline static ULONG64 ActiveProcessLinksOffset;
	inline static ULONG64 InheritedFromUniqueProcessIdOffset;
	inline static ULONG64 VadRootOffset;
};
}