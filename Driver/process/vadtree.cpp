#include "vadtree.h"
#include "process/eprocess.h"

namespace StarryEye {
//uint64_t MmVadShortData::GetStartingAddress()
//{
//	auto res = static_cast<uint64_t>(StartingVpn()) * SIZE_OF_PAGE;
//	SET_HIGH_ULONG64(res, StartingVpnHigh());
//	return res;
//}
//
//uint64_t MmVadShortData::GetEndingAddress()
//{
//	auto res = static_cast<uint64_t>(EndingVpn()) * SIZE_OF_PAGE + SIZE_OF_PAGE - 1;
//	SET_HIGH_ULONG64(res, EndingVpnHigh());
//	return res;
//}
//
//MmVadShort VadTree::Search(uint64_t address)
//{
//	return SearchRecursion(Root(), address);
//}
//
//MmVadShort VadTree::SearchRecursion(MmVadShort& root, uint64_t address)
//{
//	if (!root.IsVaild()) return {};
//	if (address < root->GetStartingAddress())
//		return SearchRecursion(root.Left(), address);
//	else if (address > root->GetEndingAddress())
//		return SearchRecursion(root.Right(), address);
//	else
//		return root;
//}
void MmVadShort::Init() {
	StartingVpnOffset = 0x18;
	EndingVpnOffset = 0x1C;
	StartingVpnHighOffset = 0x20;
	EndingVpnHighOffset = 0x21;
	ReferenceCountOffset = 0x24;
	uOffset = 0x30;
}
MmVadShort::MmVadShort(const MmVirtualAddress& vaddr) : RtlBalanceNode(vaddr) {}
uint32_t MmVadShort::StartingVpn() {
	return (vaddr_ + StartingVpnOffset).ValU32();
}
uint32_t MmVadShort::EndingVpn() {
	return (vaddr_ + EndingVpnOffset).ValU32();
}
uint8_t MmVadShort::StartingVpnHigh() {
	return (vaddr_ + StartingVpnHighOffset).ValU32();
}
uint8_t MmVadShort::EndingVpnHigh() {
	return (vaddr_ + EndingVpnHighOffset).ValU32();
}
uint64_t MmVadShort::ReferenceCount() {
	return (vaddr_ + ReferenceCountOffset).ValU64();
}
MmVadFlags MmVadShort::VadFlags() {
	return vaddr_ + uOffset;
}
MmVirtualAddress MmVadShort::StartingAddress()
{
	auto res = static_cast<uint64_t>(StartingVpn()) * SIZE_OF_PAGE;
	SET_HIGH_ULONG64(res, StartingVpnHigh());
	return res;
}
MmVirtualAddress MmVadShort::EndingAddress()
{
	auto res = static_cast<uint64_t>(EndingVpn()) * SIZE_OF_PAGE + SIZE_OF_PAGE - 1;
	SET_HIGH_ULONG64(res, EndingVpnHigh());
	return res;
}


void MmVad::Init()
{
	SubsectionOffset = 0x48;
	ViewLinksOffset = 0x60;
	VadsProcessOffset = 0x70;
}
MmVad::MmVad(const MmVirtualAddress& vaddr) : KObject(vaddr) {}
MmVadShort MmVad::Core() {
	return vaddr_;
}
SubSection MmVad::Subsection() {
	return (vaddr_ + SubsectionOffset).ValU64();
}
ListEntry<MmVad> MmVad::ViewLinks() {
	return { vaddr_ + ViewLinksOffset , ViewLinksOffset };
}


MmVadTree::MmVadTree(const MmVirtualAddress& vaddr) : RtlAvlTree(vaddr) {}
}