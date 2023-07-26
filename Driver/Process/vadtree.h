#include "Config/base.h"
#include "Config/algorithm.h"
#include "Memory/subsection.h"
#include "Process/vad_flags.h"
#include <krnlib/result.hpp>

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

	MmVadShortData(ULONG64 address);
	MmVadShortData(std::nullptr_t);
	~MmVadShortData();

	ULONG32 StartingVpn();
	ULONG32 EndingVpn();
	UCHAR StartingVpnHigh();
	UCHAR EndingVpnHigh();
	LONG64 ReferenceCount();
	MmVadFlags u();

	ULONG64 GetStartingAddress();
	ULONG64 GetEndingAddress();

	krnlib::Result<MmVad, krnlib::Empty> ConvToMmVad();

private:
	friend class MmVadData;

	static inline ULONG64 StartingVpnOffset;
	static inline ULONG64 EndingVpnOffset;
	static inline ULONG64 StartingVpnHighOffset;
	static inline ULONG64 EndingVpnHighOffset;
	static inline ULONG64 ReferenceCountOffset;
	static inline ULONG64 uOffset;
};

class MmVadData : public KObjectBase
{
public:
	static void Init();

	MmVadData(ULONG64 vadnode_addr);
	MmVadData(std::nullptr_t);
	~MmVadData();

	MmVadShortData Core();
	SubSection Subsection();
	ListEntry ViewLinks();
	EProcess VadsProcess();

private:
	friend class EProcess;

	static inline ULONG64 CoreOffset;
	static inline ULONG64 SubsectionOffset;
	static inline ULONG64 ViewLinksOffset;
	static inline ULONG64 VadsProcessOffset;
};
}

class VadTree: public RtlAvlTree<details::MmVadShortData>
{
public:
	static void Init();
	using Inherit = RtlAvlTree<details::MmVadShortData>;

	VadTree(ULONG64 address);
	VadTree(std::nullptr_t);
	
	//TODO ´ý²âÊÔ
	MmVadShort Search(ULONG64 address);

	~VadTree();

private:
	MmVadShort SearchRecursion(MmVadShort& root, ULONG64 address);
};

}