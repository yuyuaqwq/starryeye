#include "vadtree.h"

using namespace StarryEye;

void StarryEye::MmVadShort::Init()
{
	StartingVpnOffset = 0x18;
	EndingVpnOffset = 0x1C;
	ReferenceCountOffset = 0x24;
}

StarryEye::MmVadShort::MmVadShort(ULONG64 address): KObjectBase(address) {}
StarryEye::MmVadShort::MmVadShort(std::nullptr_t) : KObjectBase(nullptr) {}
StarryEye::MmVadShort::~MmVadShort() {}

ULONG32 StarryEye::MmVadShort::StartingVpn()
{
	return *(PULONG32)(address_ + StartingVpnOffset);
}

ULONG32 StarryEye::MmVadShort::EndingVpn()
{
	return *(PULONG32)(address_ + EndingVpnOffset);
}

LONG64 StarryEye::MmVadShort::ReferenceCount()
{
	return *(PLONG64)(address_ + ReferenceCountOffset);
}



void StarryEye::MmVad::Init()
{
	MmVadShort::Init();
	CoreOffset = 0;
}

StarryEye::MmVad::MmVad(ULONG64 vadnode_addr) : KObjectBase(vadnode_addr) {}
StarryEye::MmVad::MmVad(std::nullptr_t) : KObjectBase(nullptr) {}
StarryEye::MmVad::~MmVad() {}

MmVadShort StarryEye::MmVad::Core()
{
	return MmVadShort(address_ + CoreOffset);
}
