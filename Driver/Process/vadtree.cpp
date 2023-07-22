#include "vadtree.h"

namespace StarryEye {
namespace details {
void MmVadShortData::Init()
{
	StartingVpnOffset = 0x18;
	EndingVpnOffset = 0x1C;
	StartingVpnHighOffset = 0x20;
	EndingVpnHighOffset = 0x21;
	ReferenceCountOffset = 0x24;
}

MmVadShortData::MmVadShortData(ULONG64 address) : KObjectBase(address) {}
MmVadShortData::MmVadShortData(std::nullptr_t) : KObjectBase(nullptr) {}
MmVadShortData::~MmVadShortData() {}

ULONG32 MmVadShortData::StartingVpn()
{
	return *(PULONG32)(address_ + StartingVpnOffset);
}

ULONG32 MmVadShortData::EndingVpn()
{
	return *(PULONG32)(address_ + EndingVpnOffset);
}

UCHAR MmVadShortData::StartingVpnHigh()
{
	return *(PUCHAR)(address_ + StartingVpnHighOffset);
}

UCHAR MmVadShortData::EndingVpnHigh()
{
	return *(PUCHAR)(address_ + EndingVpnHighOffset);
}

ULONG64 MmVadShortData::GetStartingAddress()
{
	auto res = static_cast<ULONG64>(StartingVpn()) * SIZE_OF_PAGE;
	SET_HIGH_ULONG64(res, StartingVpnHigh());
	return res;
}

ULONG64 MmVadShortData::GetEndingAddress()
{
	auto res = static_cast<ULONG64>(EndingVpn()) * SIZE_OF_PAGE + SIZE_OF_PAGE - 1;
	SET_HIGH_ULONG64(res, EndingVpnHigh());
	return res;
}

LONG64 MmVadShortData::ReferenceCount()
{
	return *(PLONG64)(address_ + ReferenceCountOffset);
}

void MmVadData::Init()
{
	CoreOffset = 0;
}

MmVadData::MmVadData(ULONG64 vadnode_addr) : KObjectBase(vadnode_addr) {}
MmVadData::MmVadData(std::nullptr_t) : KObjectBase(nullptr) {}
MmVadData::~MmVadData() {}

MmVadShortData MmVadData::Core()
{
	return MmVadShortData(address_ + CoreOffset);
}
}

void VadTree::Init()
{
	details::MmVadData::Init();
	details::MmVadShortData::Init();
}

VadTree::VadTree(ULONG64 address) : Inherit(address) {}
VadTree::VadTree(std::nullptr_t) : Inherit(nullptr) {}
VadTree::~VadTree() {}

MmVad VadTree::Search(ULONG64 address)
{
	return SearchRecursion(Root(), address);
}

MmVad VadTree::SearchRecursion(MmVad& root, ULONG64 address)
{
	if (!root.IsVaild()) return nullptr;
	if (address < root->Core().GetStartingAddress())
		return SearchRecursion(root.Left(), address);
	else if (address > root->Core().GetEndingAddress())
		return SearchRecursion(root.Right(), address);
	else
		return root;
}
}