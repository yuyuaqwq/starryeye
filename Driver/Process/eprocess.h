#pragma once
#include "Config/base.h"
#include "Process/kprocess.h"
#include "Thread/ethread.h"
#include "Handle/handle_table.h"
#include "Config/algorithm.h"

namespace StarryEye {
	class EProcess: public KObjectBase
	{
	public:
		static void Init();

		EProcess(ULONG64 address);
		EProcess(std::nullptr_t);
		~EProcess();

		PCHAR ImageFileName();
		KProcess Pcb();
		HandleTable ObjectTable();
		KObjListEntry<EThread> ThreadListHead();

	private:
		inline static ULONG64 ImageFileNameOffset;
		inline static ULONG64 ObjectTableOffset;
		inline static ULONG64 ThreadListHeadOffset;
	};
}