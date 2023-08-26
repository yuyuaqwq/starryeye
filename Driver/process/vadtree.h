#include "config/base.h"
#include "config/algorithm.h"
#include "memory/subsection.h"
#include "process/vad_flags.h"
#include <fustd/generic/option.hpp>

namespace StarryEye {
#define SIZE_OF_PAGE 0x1000

class EProcess;
namespace details {
	class MmVadData;
	class MmVadShortData;
}

using MmVad = RtlBalanceNode<details::MmVadData>;
using MmVadShort = RtlBalanceNode<details::MmVadShortData>;

namespace details {

class MmVadShortData : public KObjectBase
{
public:
	static void Init();

	MmVadShortData(uint64_t address);
	MmVadShortData() = default;
	~MmVadShortData() = default;

	ULONG32 StartingVpn();
	ULONG32 EndingVpn();
	uint8_t StartingVpnHigh();
	uint8_t EndingVpnHigh();
	LONG64 ReferenceCount();
	MmVadFlags u();

	uint64_t GetStartingAddress();
	uint64_t GetEndingAddress();

	fustd::Option<MmVad> ConvToMmVad();

private:
	friend class MmVadData;

	static inline uint64_t StartingVpnOffset;
	static inline uint64_t EndingVpnOffset;
	static inline uint64_t StartingVpnHighOffset;
	static inline uint64_t EndingVpnHighOffset;
	static inline uint64_t ReferenceCountOffset;
	static inline uint64_t uOffset;
};

class MmVadData : public KObjectBase
{
public:
	static void Init();

	MmVadData(uint64_t vadnode_addr);
	MmVadData() = default;
	~MmVadData() = default;

	MmVadShortData Core();
	SubSection Subsection();
	ListEntry ViewLinks();
	EProcess VadsProcess();

private:
	friend class EProcess;

	static inline uint64_t CoreOffset;
	static inline uint64_t SubsectionOffset;
	static inline uint64_t ViewLinksOffset;
	static inline uint64_t VadsProcessOffset;
};
}

class VadTree: public RtlAvlTree<details::MmVadShortData>
{
public:
	static void Init();
	using Inherit = RtlAvlTree<details::MmVadShortData>;

	VadTree(uint64_t address);
	VadTree() = default;
	~VadTree() = default;
	
	//TODO ´ý²âÊÔ
	MmVadShort Search(uint64_t address);

private:
	MmVadShort SearchRecursion(MmVadShort& root, uint64_t address);
};
}