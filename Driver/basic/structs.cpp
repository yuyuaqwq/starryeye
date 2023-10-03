#include "structs.h"

namespace stareye {
KObject::KObject(MmVirtualAddress vaddr): vaddr_(vaddr) {}
MmVirtualAddress KObject::VAddr() const {
	return vaddr_;
}

bool KObject::IsValid() const {
	return vaddr_.IsValid();
}

RtlBalanceNode::RtlBalanceNode(MmVirtualAddress vaddr) : KObject(vaddr) {}
RtlBalanceNode RtlBalanceNode::Left() const {
	return MmVirtualAddress(vaddr_.Pointer<RTL_BALANCED_NODE>()->Left, vaddr_.Owner());
}
RtlBalanceNode RtlBalanceNode::Right() const {
	return MmVirtualAddress(vaddr_.Pointer<RTL_BALANCED_NODE>()->Right, vaddr_.Owner());
}
RtlBalanceNode RtlBalanceNode::Parent() const {
	return MmVirtualAddress(vaddr_.Pointer<RTL_BALANCED_NODE>()->ParentValue & ~0b111ull, vaddr_.Owner());
}
bool RtlBalanceNode::HasLeft() const {
	return Left().IsValid();
}
bool RtlBalanceNode::HasRight() const {
	return Right().IsValid();
}
bool RtlBalanceNode::HasParent() const {
	return Parent().IsValid();
}
bool operator==(const RtlBalanceNode& x, const RtlBalanceNode& y) noexcept {
	return x.vaddr_ == y.vaddr_;
}
bool operator!=(const RtlBalanceNode& x, const RtlBalanceNode& y) noexcept {
	return x.vaddr_ != y.vaddr_;
}


RtlAvlTree::RtlAvlTree(MmVirtualAddress vaddr) : KObject(vaddr) {}
RtlBalanceNode RtlAvlTree::Root() const {
	return MmVirtualAddress(vaddr_.ValU64(), vaddr_.Owner());
}
RtlAvlTree::iterator RtlAvlTree::begin() const {
	auto tmp = Root();
	while (tmp.HasLeft()) {
		tmp = tmp.Left();
	}
	return tmp;
}
RtlAvlTree::iterator RtlAvlTree::end() const {
	auto tmp = Root();
	while (tmp.HasRight()) {
		tmp = tmp.Right();
	}
	return tmp;
}

namespace details {
RtlAvlTreeIterator::RtlAvlTreeIterator(const RtlBalanceNode& cur) : cur_(cur) {}
RtlBalanceNode& RtlAvlTreeIterator::operator*() { return cur_; }
RtlBalanceNode* RtlAvlTreeIterator::operator->() { return &cur_; }
RtlAvlTreeIterator& RtlAvlTreeIterator::operator++() {
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
RtlAvlTreeIterator& RtlAvlTreeIterator::operator--() {
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
RtlAvlTreeIterator RtlAvlTreeIterator::operator++(int) {
	auto tmp = *this;
	++(*this);
	return tmp;
}
RtlAvlTreeIterator RtlAvlTreeIterator::operator--(int) {
	auto tmp = *this;
	--(*this);
	return tmp;
}
bool RtlAvlTreeIterator::operator==(const RtlAvlTreeIterator& x) const {
	return cur_ == x.cur_;
}
bool RtlAvlTreeIterator::operator!=(const RtlAvlTreeIterator& x) const {
	return cur_ != x.cur_;
}
void RtlAvlTreeIterator::GoLeftMost(const RtlBalanceNode& node) {
	RtlBalanceNode tmp = node;
	while (tmp.HasLeft()) {
		tmp = tmp.Left();
	}
	cur_ = tmp;
}
void RtlAvlTreeIterator::GoRightMost(const RtlBalanceNode& node) {
	RtlBalanceNode tmp = node;
	while (tmp.HasRight()) {
		tmp = tmp.Right();
	}
	cur_ = tmp;
}
}
}