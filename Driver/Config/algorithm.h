#pragma once
#include <ntifs.h>
#include "Config/ynstd.h"

namespace StarryEye {
	template<class KObjT>
	class KObjListEntry
	{
	public:
		using Self = KObjListEntry<KObjT>;

		KObjListEntry(ULONG64 list_addr, ULONG64 offset)
		{
			list_ = (PLIST_ENTRY64)list_addr;
			offset_ = offset;
		}
		~KObjListEntry() {}

		Self Flink()
		{
			return Self((ULONG64)list_->Flink, offset_);
		}
		Self Blink()
		{
			return Self((ULONG64)list_->Blink, offset_);
		}

		KObjT Object()
		{
			return KObjT((ULONG64)list_ - offset_);
		}

		bool IsVaild()
		{
			return MmIsAddressValid(list_);
		}

	private:
		PLIST_ENTRY64 list_;
		ULONG64 offset_;
	};
}