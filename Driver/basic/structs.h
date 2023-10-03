#pragma once

#include <optional>
#include <functional>
#include "virtual_addr.h"

namespace stareye {
class KObject {
public:
	KObject() = default;
	KObject(MmVirtualAddress vaddr);
	MmVirtualAddress VAddr() const;
	bool IsValid() const;

protected:
	MmVirtualAddress vaddr_;
};

template<class T>
concept InheritKObjectT = std::is_convertible_v<T*, KObject*>;

class RtlBalanceNode: public KObject
{
public:
	RtlBalanceNode(MmVirtualAddress vaddr);
	RtlBalanceNode() = default;
	~RtlBalanceNode() = default;

	RtlBalanceNode Left() const;
	RtlBalanceNode Right() const;
	RtlBalanceNode Parent() const;

	bool HasLeft() const;
	bool HasRight() const;
	bool HasParent() const;

	friend bool operator==(const RtlBalanceNode& x, const RtlBalanceNode& y) noexcept;
	friend bool operator!=(const RtlBalanceNode& x, const RtlBalanceNode& y) noexcept;

	template<class T>
		requires std::is_convertible_v<T*, RtlBalanceNode*>
	T Impl() {
		return vaddr_;
	}
};

namespace details {
class RtlAvlTreeIterator
{
public:
	using difference_type = std::ptrdiff_t;
	using value_type = RtlBalanceNode;
	using pointer = value_type*;
	using reference = value_type&;
	using iterator_category = std::bidirectional_iterator_tag;

	RtlAvlTreeIterator(const RtlBalanceNode& cur);
	~RtlAvlTreeIterator() = default;

	RtlBalanceNode& operator*();
	RtlBalanceNode* operator->();
	RtlAvlTreeIterator& operator++();
	RtlAvlTreeIterator& operator--();
	RtlAvlTreeIterator operator++(int);
	RtlAvlTreeIterator operator--(int);
	bool operator==(const RtlAvlTreeIterator& x) const;
	bool operator!=(const RtlAvlTreeIterator& x) const;

private:
	void GoLeftMost(const RtlBalanceNode& node);
	void GoRightMost(const RtlBalanceNode& node);

	RtlBalanceNode cur_;
};
}


class RtlAvlTree : public KObject
{
public:
	using iterator = details::RtlAvlTreeIterator;

	RtlAvlTree(MmVirtualAddress vaddr);
	RtlAvlTree() = default;
	~RtlAvlTree() = default;

	RtlBalanceNode Root() const;

	iterator begin() const;
	iterator end() const;
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
public:
	ExFastRef() = default;
	ExFastRef(MmVirtualAddress vaddr) : KObject(vaddr) {}
	~ExFastRef() = default;

	ObjectT Object() {
		return vaddr_.Value<uint64_t>() & ~0xFull;
	}

	uint64_t RefCnt() {
		return vaddr_.Value<uint64_t>() & 0xFull;
	}
};
}