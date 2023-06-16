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

ULONG64 StarryEye::MmVadShort::StartingAddress()
{
	return ULONG64(StartingVpn()) << 12;
}

ULONG64 StarryEye::MmVadShort::EndingAddress()
{
	return ULONG64(EndingVpn()) << 12;
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


StarryEye::VadTree::VadTree(ULONG64 address): Inherit(address) {}
StarryEye::VadTree::VadTree(std::nullptr_t) : Inherit(nullptr) {}
StarryEye::VadTree::~VadTree() {}

VadNode StarryEye::VadTree::Search(ULONG64 address)
{
	return SearchRecursion(Root(), address);
}

VadNode StarryEye::VadTree::SearchRecursion(VadNode& root, ULONG64 address)
{
	if (!root.IsVaild()) return nullptr;
	if (address < root->Core().StartingAddress())
		return SearchRecursion(root.Left(), address);
	else if (address > root->Core().EndingAddress())
		return SearchRecursion(root.Right(), address);
	else
		return root;
}
