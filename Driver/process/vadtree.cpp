#include "vadtree.h"
#include "process/eprocess.h"

namespace StarryEye {
namespace details {
void MmVadShortData::Init()
{
	StartingVpnOffset = 0x18;
	EndingVpnOffset = 0x1C;
	StartingVpnHighOffset = 0x20;
	EndingVpnHighOffset = 0x21;
	ReferenceCountOffset = 0x24;
	uOffset = 0x30;
}

MmVadShortData::MmVadShortData(uint64_t address) : KObjectBase(address) {}

ULONG32 MmVadShortData::StartingVpn()
{
	return *(PULONG32)(address_ + StartingVpnOffset);
}

ULONG32 MmVadShortData::EndingVpn()
{
	return *(PULONG32)(address_ + EndingVpnOffset);
}

uint8_t MmVadShortData::StartingVpnHigh()
{
	return *(PUCHAR)(address_ + StartingVpnHighOffset);
}

uint8_t MmVadShortData::EndingVpnHigh()
{
	return *(PUCHAR)(address_ + EndingVpnHighOffset);
}

uint64_t MmVadShortData::GetStartingAddress()
{
	auto res = static_cast<uint64_t>(StartingVpn()) * SIZE_OF_PAGE;
	SET_HIGH_ULONG64(res, StartingVpnHigh());
	return res;
}

uint64_t MmVadShortData::GetEndingAddress()
{
	auto res = static_cast<uint64_t>(EndingVpn()) * SIZE_OF_PAGE + SIZE_OF_PAGE - 1;
	SET_HIGH_ULONG64(res, EndingVpnHigh());
	return res;
}

fustd::Option<MmVad> MmVadShortData::ConvToMmVad()
{
	if (u().PrivateMemory() == 0)
		return fustd::Some(MmVad(address_));
	else
		return fustd::None();
}

LONG64 MmVadShortData::ReferenceCount()
{
	return *(PLONG64)(address_ + ReferenceCountOffset);
}

MmVadFlags MmVadShortData::u()
{
	return MmVadFlags(address_ + uOffset);
}

void MmVadData::Init()
{
	CoreOffset = 0;
	SubsectionOffset = 0x48;
	ViewLinksOffset = 0x60;
	VadsProcessOffset = 0x70;
}

MmVadData::MmVadData(uint64_t vadnode_addr) : KObjectBase(vadnode_addr) {}

MmVadShortData MmVadData::Core()
{
	return MmVadShortData(address_ + CoreOffset);
}

ListEntry MmVadData::ViewLinks()
{
	return ListEntry(address_ + ViewLinksOffset, VadsProcessOffset);
}

SubSection MmVadData::Subsection()
{
	return SubSection(*(uint64_t*)(address_ + SubsectionOffset));
}

EProcess MmVadData::VadsProcess()
{
	return EProcess(*(uint64_t*)(address_ + VadsProcessOffset));
}
}

void VadTree::Init()
{
	details::MmVadData::Init();
	details::MmVadShortData::Init();
	MmVadFlags::Init();
}

VadTree::VadTree(uint64_t address) : Inherit(address) {}

MmVadShort VadTree::Search(uint64_t address)
{
	return SearchRecursion(Root(), address);
}

MmVadShort VadTree::SearchRecursion(MmVadShort& root, uint64_t address)
{
	if (!root.IsVaild()) return nullptr;
	if (address < root->GetStartingAddress())
		return SearchRecursion(root.Left(), address);
	else if (address > root->GetEndingAddress())
		return SearchRecursion(root.Right(), address);
	else
		return root;
}
}