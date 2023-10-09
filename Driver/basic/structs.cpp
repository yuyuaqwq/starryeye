#include "structs.h"

namespace stareye {
KObject::KObject(MmVirtualAddress vaddr) noexcept: vaddr_(vaddr) {}
MmVirtualAddress KObject::VAddr() const noexcept {
	return vaddr_;
}

bool KObject::IsValid() const noexcept {
	return vaddr_.IsValid();
}

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
RtlAvlTree::const_iterator RtlAvlTree::begin() const {
	return BegNode();
}
RtlAvlTree::const_iterator RtlAvlTree::end() const {
	return EndNode();
}
RtlAvlTree::iterator RtlAvlTree::begin() {
	return BegNode();
}
RtlAvlTree::iterator RtlAvlTree::end() {
	return EndNode();
}
RtlBalanceNode RtlAvlTree::BegNode() const {
	auto tmp = Root();
	while (tmp.HasLeft()) {
		tmp = tmp.Left();
	}
	return tmp;
}
RtlBalanceNode RtlAvlTree::EndNode() const {
	auto tmp = Root();
	while (tmp.HasRight()) {
		tmp = tmp.Right();
	}
	return tmp;
}

namespace details {
RtlAvlTreeConstIterator::RtlAvlTreeConstIterator(const RtlBalanceNode& cur) : cur_(cur) {}
RtlAvlTreeConstIterator::reference RtlAvlTreeConstIterator::operator*() const noexcept {
	return cur_;
}
RtlAvlTreeConstIterator::pointer RtlAvlTreeConstIterator::operator->() const noexcept {
	return &cur_;
}
RtlAvlTreeConstIterator& RtlAvlTreeConstIterator::operator++() {
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
RtlAvlTreeConstIterator& RtlAvlTreeConstIterator::operator--() {
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
RtlAvlTreeConstIterator RtlAvlTreeConstIterator::operator++(int) {
	auto tmp = *this;
	++(*this);
	return tmp;
}
RtlAvlTreeConstIterator RtlAvlTreeConstIterator::operator--(int) {
	auto tmp = *this;
	--(*this);
	return tmp;
}
bool RtlAvlTreeConstIterator::operator==(const RtlAvlTreeConstIterator& x) const noexcept {
	return cur_ == x.cur_;
}
void RtlAvlTreeConstIterator::GoLeftMost(const RtlBalanceNode& node) {
	RtlBalanceNode tmp = node;
	while (tmp.HasLeft()) {
		tmp = tmp.Left();
	}
	cur_ = tmp;
}
void RtlAvlTreeConstIterator::GoRightMost(const RtlBalanceNode& node) {
	RtlBalanceNode tmp = node;
	while (tmp.HasRight()) {
		tmp = tmp.Right();
	}
	cur_ = tmp;
}


RtlAvlTreeIterator::reference RtlAvlTreeIterator::operator*() const noexcept {
	return const_cast<reference>(InheritT::operator*());
}
RtlAvlTreeIterator::pointer RtlAvlTreeIterator::operator->() const noexcept {
	return const_cast<pointer>(InheritT::operator->());
}
RtlAvlTreeIterator& RtlAvlTreeIterator::operator++() {
	InheritT::operator++();
	return *this;
}
RtlAvlTreeIterator& RtlAvlTreeIterator::operator--() {
	InheritT::operator--();
	return *this;
}
RtlAvlTreeIterator RtlAvlTreeIterator::operator++(int) {
	auto tmp = *this;
	InheritT::operator++();
	return tmp;
}
RtlAvlTreeIterator RtlAvlTreeIterator::operator--(int) {
	auto tmp = *this;
	InheritT::operator--();
	return tmp;
}
bool RtlAvlTreeIterator::operator==(const RtlAvlTreeIterator& x) const noexcept {
	return InheritT::operator==(x);
}
}
}