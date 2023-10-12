#pragma once

#include <optional>
#include <functional>
#include "virtual_addr.h"

#define STAREYE_USING_BASE(base) \
public:\
using BaseT = base;\
using BaseT::BaseT;

namespace stareye {
class KObject {
public:
	KObject() = default;
	~KObject() = default;
	KObject(MmVirtualAddress vaddr) noexcept;
	MmVirtualAddress VAddr() const noexcept;

protected:
	MmVirtualAddress vaddr_;
};

inline bool IsValid(const KObject& obj) {
	return IsValid(obj.VAddr());
}

template<class T>
concept InheritKObjectT = std::is_convertible_v<T*, KObject*>;


template<class DeriveT>
class RtlBalanceNode: public KObject
{
	STAREYE_USING_BASE(KObject)
public:
	DeriveT Left() const {
		return MmVirtualAddress(vaddr_.Pointer<RTL_BALANCED_NODE>()->Left, vaddr_.Owner());
	}
	DeriveT Right() const {
		return MmVirtualAddress(vaddr_.Pointer<RTL_BALANCED_NODE>()->Right, vaddr_.Owner());
	}
	DeriveT Parent() const {
		return MmVirtualAddress(vaddr_.Pointer<RTL_BALANCED_NODE>()->ParentValue & ~0b111ull, vaddr_.Owner());
	}

	friend bool operator==(const RtlBalanceNode& x, const RtlBalanceNode& y) noexcept {
		return x.vaddr_ == y.vaddr_;
	}
};

namespace details {
template<class NodeT>
class RtlAvlTreeConstIterator
{
public:
	using difference_type = std::ptrdiff_t;
	using value_type = NodeT;
	using pointer = const value_type*;
	using reference = const value_type&;
	using iterator_category = std::bidirectional_iterator_tag;

	RtlAvlTreeConstIterator(const NodeT& cur) : cur_(cur) {}
	~RtlAvlTreeConstIterator() = default;

	reference operator*() const noexcept { return cur_; }
	pointer operator->() const noexcept { return &cur_; }
	RtlAvlTreeConstIterator& operator++() {
		if (IsValid(cur_.Left())) {
			GoLeftMost(cur_.Right());
		}
		else {
			while (IsValid(cur_.Parent()) && cur_.Parent().Right() == cur_) {
				cur_ = cur_.Parent();
			}
			cur_ = cur_.Parent();
		}
		return *this;
	}
	RtlAvlTreeConstIterator& operator--() {
		if (IsValid(cur_.Left())) {
			GoRightMost(cur_.Left());
		}
		else {
			while (IsValid(cur_.Parent()) && cur_.Parent().Left() == cur_) {
				cur_ = cur_.Parent();
			}
			cur_ = cur_.Parent();
		}
		return *this;
	}
	RtlAvlTreeConstIterator operator++(int) {
		auto tmp = *this;
		++(*this);
		return tmp;
	}
	RtlAvlTreeConstIterator operator--(int) {
		auto tmp = *this;
		--(*this);
		return tmp;
	}
	bool operator==(const RtlAvlTreeConstIterator& x) const noexcept {
		return cur_ == x.cur_;
	}

private:
	void GoLeftMost(const NodeT& node) {
		NodeT tmp = node;
		while (IsValid(tmp.Left())) {
			tmp = tmp.Left();
		}
		cur_ = tmp;
	}
	void GoRightMost(const NodeT& node) {
		NodeT tmp = node;
		while (IsValid(tmp.Right())) {
			tmp = tmp.Right();
		}
		cur_ = tmp;
	}

	NodeT cur_;
};

template<class NodeT>
class RtlAvlTreeIterator : public RtlAvlTreeConstIterator<NodeT>
{
	STAREYE_USING_BASE(RtlAvlTreeConstIterator<NodeT>)
public:
	using difference_type = std::ptrdiff_t;
	using value_type = NodeT;
	using pointer = value_type*;
	using reference = value_type&;
	using iterator_category = std::bidirectional_iterator_tag;

	reference operator*() const noexcept {
		return const_cast<reference>(BaseT::operator*());
	}
	pointer operator->() const noexcept {
		return const_cast<pointer>(BaseT::operator->());
	}
	RtlAvlTreeIterator& operator++() {
		BaseT::operator++();
		return *this;
	}
	RtlAvlTreeIterator& operator--() {
		BaseT::operator--();
		return *this;
	}
	RtlAvlTreeIterator operator++(int) {
		auto tmp = *this;
		BaseT::operator++();
		return tmp;
	}
	RtlAvlTreeIterator operator--(int) {
		auto tmp = *this;
		BaseT::operator--();
		return tmp;
	}
	bool operator==(const RtlAvlTreeIterator& x) const noexcept {
		return BaseT::operator==(x);
	}
};
}

template<class NodeT>
class RtlAvlTree : public KObject
{
	STAREYE_USING_BASE(KObject)
public:
	using value_type = NodeT;
	using iterator = details::RtlAvlTreeIterator<NodeT>;
	using const_iterator = details::RtlAvlTreeConstIterator<NodeT>;

	NodeT Root() const {
		return MmVirtualAddress(vaddr_.ValU64(), vaddr_.Owner());
	}

	const_iterator begin() const {
		return BegNode();
	}
	const_iterator end() const {
		return EndNode();
	}
	iterator begin() {
		return BegNode();
	}
	iterator end() {
		return EndNode();
	}

private:
	NodeT BegNode() const {
		auto tmp = Root();
		while (IsValid(tmp.Left())) {
			tmp = tmp.Left();
		}
		return tmp;
	}
	NodeT EndNode() const {
		auto tmp = Root();
		while (IsValid(tmp.Right())) {
			tmp = tmp.Right();
		}
		return tmp;
	}
};



template<class T>
class ListEntry : public KObject
{
public:
	ListEntry() = default;
	ListEntry(MmVirtualAddress vaddr, ptrdiff_t off_to_head): KObject(vaddr), off_to_head_(off_to_head) {}
	~ListEntry() = default;

	ListEntry Blink() {
		return vaddr_.Value<LIST_ENTRY64>().Blink;
	}

	ListEntry Flink() {
		return vaddr_.Value<LIST_ENTRY64>().Flink;
	}

	T Object() {
		return vaddr_ - off_to_head_;
	}

private:
	ptrdiff_t off_to_head_;
};

template<class ObjectT>
class ExFastRef : public KObject
{
	STAREYE_USING_BASE(KObject)
public:
	ObjectT Object() {
		return vaddr_.Value<uint64_t>() & ~0xFull;
	}

	uint64_t RefCnt() {
		return vaddr_.Value<uint64_t>() & 0xFull;
	}
};
}