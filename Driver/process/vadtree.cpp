#include "vadtree.h"
//#include "eprocess.h"

namespace stareye {
void MmVadShort::Init() {
	StartingVpnOffset = 0x18;
	EndingVpnOffset = 0x1C;
	StartingVpnHighOffset = 0x20;
	EndingVpnHighOffset = 0x21;
	ReferenceCountOffset = 0x24;
	uOffset = 0x30;
}
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
	uint64_t res = StartingVpn();
	fustd::SetIntegerHigh<4>(res, StartingVpnHigh());
	res <<= PAGE_SHIFT;
	return MmVirtualAddress(res, vaddr_.Owner());
}
MmVirtualAddress MmVadShort::EndingAddress()
{
	uint64_t res = EndingVpn();
	fustd::SetIntegerHigh<4>(res, EndingVpnHigh());
	res <<= PAGE_SHIFT;
	res += PAGE_SIZE - 1;
	return MmVirtualAddress(res, vaddr_.Owner());
}


void MmVad::Init()
{
	SubsectionOffset = 0x48;
	ViewLinksOffset = 0x60;
	VadsProcessOffset = 0x70;
}
MmVadShort MmVad::Core() {
	return vaddr_;
}
SubSection MmVad::Subsection() {
	return (vaddr_ + SubsectionOffset).DerefAsAddr();
}
ListEntry<MmVad> MmVad::ViewLinks() {
	return ListEntry<MmVad>(vaddr_ + ViewLinksOffset, ViewLinksOffset);
}


void MmVadTree::Init()
{
	MmVadShort::Init();
	MmVad::Init();
}

std::optional<MmVadShort> MmVadTree::SearchNode(MmVirtualAddress vaddr)
{
	auto cur_vad = Root();
	while (IsValid(cur_vad)) {
		if (cur_vad.StartingAddress() > vaddr) {
			cur_vad = cur_vad.Left();
		}
		else if (cur_vad.EndingAddress() < vaddr) {
			cur_vad = cur_vad.Right();
		}
		else return cur_vad;
	}
	return std::nullopt;
}
}