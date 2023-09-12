#include "vadtree.h"
//#include "eprocess.h"

namespace StarryEye {
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
	return ListEntry<MmVad>(vaddr_ + ViewLinksOffset, ViewLinksOffset);
}


void MmVadTree::Init()
{
	MmVadShort::Init();
	MmVad::Init();
}

MmVadTree::MmVadTree(const MmVirtualAddress& vaddr) : RtlAvlTree(vaddr) {}
fustd::Option<MmVadShort> MmVadTree::SearchNode(const MmVirtualAddress& vaddr)
{
	auto cur_vad = Root().Impl<MmVadShort>();
	while (cur_vad.VAddr().IsValid()) {
		if (cur_vad.StartingAddress() > vaddr) {
			cur_vad = cur_vad.Left().Impl<MmVadShort>();
		}
		else if (cur_vad.EndingAddress() < vaddr) {
			cur_vad = cur_vad.Right().Impl<MmVadShort>();
		}
		else return fustd::Some(std::move(cur_vad));
	}
	return fustd::None();
}
}