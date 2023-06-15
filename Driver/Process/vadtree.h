#include "Config/base.h"
#include "Config/algorithm.h"

namespace StarryEye {
class EProcess;
class MmVad;

class MmVadShort : public KObjectBase
{
public:
	MmVadShort(ULONG64 address);
	MmVadShort(std::nullptr_t);
	~MmVadShort();

	ULONG32 StartingVpn();
	ULONG32 EndingVpn();
	LONG64 ReferenceCount();

private:
	friend class MmVad;

	static void Init();

	static inline ULONG64 StartingVpnOffset;
	static inline ULONG64 EndingVpnOffset;
	static inline ULONG64 ReferenceCountOffset;
};

class MmVad: public KObjectBase
{
public:
	MmVad(ULONG64 vadnode_addr);
	MmVad(std::nullptr_t);
	~MmVad();

	MmVadShort Core();
		
private:
	friend class EProcess;

	static void Init();

	static inline ULONG64 CoreOffset;
};

using VadTree = RtlAvlTree<MmVad>;
}