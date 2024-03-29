#pragma once
#include "memory/subsection.h"
#include "vad_flags.h"

namespace stareye {
class MmVadShort : public RtlBalanceNode<MmVadShort>
{
	STAREYE_USING_BASE(RtlBalanceNode<MmVadShort>)
public:
	static void Init();

	uint32_t StartingVpn();
	uint32_t EndingVpn();
	uint8_t StartingVpnHigh();
	uint8_t EndingVpnHigh();
	uint64_t ReferenceCount();
	MmVadFlags VadFlags();

	MmVirtualAddress StartingAddress();
	MmVirtualAddress EndingAddress();

private:
	static inline uint64_t StartingVpnOffset;
	static inline uint64_t EndingVpnOffset;
	static inline uint64_t StartingVpnHighOffset;
	static inline uint64_t EndingVpnHighOffset;
	static inline uint64_t ReferenceCountOffset;
	static inline uint64_t uOffset;
};

class MmVad : public KObject
{
public:
	static void Init();

	using KObject::KObject;

	MmVadShort Core();
	SubSection Subsection();
	ListEntry<MmVad> ViewLinks();

private:
	friend class EProcess;

	static inline uint64_t SubsectionOffset;
	static inline uint64_t ViewLinksOffset;
	static inline uint64_t VadsProcessOffset;
};

class MmVadTree: public RtlAvlTree<MmVadShort>
{
	STAREYE_USING_BASE(RtlAvlTree<MmVadShort>)
public:
	static void Init();

	std::optional<MmVadShort> SearchNode(MmVirtualAddress vaddr);
};
}