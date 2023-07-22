#include "Config/base.h"
#include "Config/algorithm.h"
#include "Memory/subsection.h"

namespace StarryEye {
#define SIZE_OF_PAGE 0x1000

class EProcess;

namespace details {
class MmVadData;

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
	ULONG64 GetStartingAddress();
	ULONG64 GetEndingAddress();
	LONG64 ReferenceCount();

private:
	friend class MmVadData;

	static inline ULONG64 StartingVpnOffset;
	static inline ULONG64 EndingVpnOffset;
	static inline ULONG64 StartingVpnHighOffset;
	static inline ULONG64 EndingVpnHighOffset;
	static inline ULONG64 ReferenceCountOffset;
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
	ULONG64 VadsProcessAddress();

private:
	friend class EProcess;

	static inline ULONG64 CoreOffset;
	static inline ULONG64 SubsectionOffset;
	static inline ULONG64 ViewLinksOffset;
	static inline ULONG64 VadsProcessOffset;
};
}

using MmVad = RtlBalanceNode<details::MmVadData>;
using MmVadShort = RtlBalanceNode<details::MmVadShortData>;

class VadTree: public RtlAvlTree<details::MmVadData>
{
public:
	static void Init();
	using Inherit = RtlAvlTree<details::MmVadData>;

	VadTree(ULONG64 address);
	VadTree(std::nullptr_t);
	
	//TODO ������
	MmVad Search(ULONG64 address);

	~VadTree();

private:
	MmVad SearchRecursion(MmVad& root, ULONG64 address);
};

}