#include "structs.h"

namespace stareye {
KObject::KObject(const MmVirtualAddress& vaddr): vaddr_(vaddr) {}
const MmVirtualAddress& KObject::VAddr() const {
	return vaddr_;
}

RtlBalanceNode::RtlBalanceNode(const MmVirtualAddress& vaddr) : KObject(vaddr) {}
RtlBalanceNode RtlBalanceNode::Left() const {
	return MmVirtualAddress(vaddr_.Pointer<RTL_BALANCED_NODE>()->Left, vaddr_.Owner());
}
RtlBalanceNode RtlBalanceNode::Right() const {
	return MmVirtualAddress(vaddr_.Pointer<RTL_BALANCED_NODE>()->Right, vaddr_.Owner());
}
RtlBalanceNode RtlBalanceNode::Parent() const {
	return MmVirtualAddress(vaddr_.Pointer<RTL_BALANCED_NODE>()->ParentValue, vaddr_.Owner());
}
bool RtlBalanceNode::HasLeft() const {
	return Left().vaddr_.IsValid();
}
bool RtlBalanceNode::HasRight() const {
	return Right().vaddr_.IsValid();
}
bool RtlBalanceNode::HasParent() const {
	return Parent().vaddr_.IsValid();
}
bool operator==(const RtlBalanceNode& x, const RtlBalanceNode& y) noexcept {
	return x.vaddr_ == y.vaddr_;
}
bool operator!=(const RtlBalanceNode& x, const RtlBalanceNode& y) noexcept {
	return x.vaddr_ != y.vaddr_;
}


RtlAvlTree::RtlAvlTree(const MmVirtualAddress& vaddr) : KObject(vaddr) {}
RtlBalanceNode RtlAvlTree::Root() {
	return MmVirtualAddress(vaddr_.ValU64(), vaddr_.Owner());
}
void RtlAvlTree::Foreach(ForeachCallBackT callback) {
	ForeachRecursion(Root(), callback);
}
krnlib::list<RtlBalanceNode> RtlAvlTree::GetAllNodes() {
	krnlib::list<RtlBalanceNode> total;
	Foreach([&](const RtlBalanceNode& node) {
		total.push_back(node);
		return true;
		});
	return total;
}
RtlBalanceNode RtlAvlTree::BeginNode() {
	auto tmp = Root();
	while (tmp.HasLeft()) {
		tmp = tmp.Left();
	}
	return tmp;
}
RtlBalanceNode RtlAvlTree::EndNode() {
	auto tmp = Root();
	while (tmp.HasRight()) {
		tmp = tmp.Right();
	}
	return tmp;
}
RtlAvlTree::Iterator RtlAvlTree::begin() {
	return BeginNode();
}
RtlAvlTree::Iterator RtlAvlTree::end() {
	return EndNode();
}
bool RtlAvlTree::ForeachRecursion(const RtlBalanceNode& root, ForeachCallBackT callback) {
	if (!root.VAddr().IsValid()) return true;
	if (!ForeachRecursion(root.Left(), callback)) return false;
	if (!callback(root)) return false;
	return ForeachRecursion(root.Right(), callback);
}

RtlAvlTree::Iterator::Iterator(const RtlBalanceNode& cur) : cur_(cur) {}
RtlBalanceNode& RtlAvlTree::Iterator::operator*() { return cur_; }
RtlBalanceNode* RtlAvlTree::Iterator::operator->() { return &cur_; }
RtlAvlTree::Iterator& RtlAvlTree::Iterator::operator++() {
	if (cur_.HasRight()) {
		GoLeftMost(cur_.Right());
	}
	else {
		while (cur_.HasParent() && cur_.Parent().Right() == cur_) {
			cur_ = cur_.Parent();
		}
		cur_ = cur_.Parent();
	}
	return *this;
}
RtlAvlTree::Iterator& RtlAvlTree::Iterator::operator--() {
	if (cur_.HasLeft()) {
		GoRightMost(cur_.Left());
	}
	else {
		while (cur_.HasParent() && cur_.Parent().Left() == cur_) {
			cur_ = cur_.Parent();
		}
		cur_ = cur_.Parent();
	}
	return *this;
}
RtlAvlTree::Iterator RtlAvlTree::Iterator::operator++(int) {
	auto tmp = *this;
	++(*this);
	return tmp;
}
RtlAvlTree::Iterator RtlAvlTree::Iterator::operator--(int) {
	auto tmp = *this;
	--(*this);
	return tmp;
}
bool RtlAvlTree::Iterator::operator==(const Iterator& x) {
	return cur_ == x.cur_;
}
bool RtlAvlTree::Iterator::operator!=(const Iterator& x) {
	return cur_ != x.cur_;
}
void RtlAvlTree::Iterator::GoLeftMost(const RtlBalanceNode& node) {
	RtlBalanceNode tmp = node;
	while (tmp.HasLeft()) {
		tmp = tmp.Left();
	}
	cur_ = tmp;
}
void RtlAvlTree::Iterator::GoRightMost(const RtlBalanceNode& node) {
	RtlBalanceNode tmp = node;
	while (tmp.HasRight()) {
		tmp = tmp.Right();
	}
	cur_ = tmp;
}
}