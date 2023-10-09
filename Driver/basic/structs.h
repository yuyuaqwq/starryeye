#pragma once

#include <optional>
#include <functional>
#include "virtual_addr.h"

namespace stareye {
class KObject {
public:
	KObject() = default;
	~KObject() = default;
	KObject(MmVirtualAddress vaddr) noexcept;
	MmVirtualAddress VAddr() const noexcept;
	bool IsValid() const noexcept;

protected:
	MmVirtualAddress vaddr_;
};

template<class T>
concept InheritKObjectT = std::is_convertible_v<T*, KObject*>;

class RtlBalanceNode: public KObject
{
public:
	using KObject::KObject;

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
class RtlAvlTreeConstIterator
{
public:
	using difference_type = std::ptrdiff_t;
	using value_type = RtlBalanceNode;
	using pointer = const value_type*;
	using reference = const value_type&;
	using iterator_category = std::bidirectional_iterator_tag;

	RtlAvlTreeConstIterator(const RtlBalanceNode& cur);
	~RtlAvlTreeConstIterator() = default;

	reference operator*() const noexcept;
	pointer operator->() const noexcept;
	RtlAvlTreeConstIterator& operator++();
	RtlAvlTreeConstIterator& operator--();
	RtlAvlTreeConstIterator operator++(int);
	RtlAvlTreeConstIterator operator--(int);
	bool operator==(const RtlAvlTreeConstIterator& x) const noexcept;

private:
	void GoLeftMost(const RtlBalanceNode& node);
	void GoRightMost(const RtlBalanceNode& node);

	RtlBalanceNode cur_;
};

class RtlAvlTreeIterator : public RtlAvlTreeConstIterator
{
public:
	using RtlAvlTreeConstIterator::RtlAvlTreeConstIterator;
	using InheritT = RtlAvlTreeConstIterator;

	using difference_type = std::ptrdiff_t;
	using value_type = RtlBalanceNode;
	using pointer = value_type*;
	using reference = value_type&;
	using iterator_category = std::bidirectional_iterator_tag;

	reference operator*() const noexcept;
	pointer operator->() const noexcept;
	RtlAvlTreeIterator& operator++();
	RtlAvlTreeIterator& operator--();
	RtlAvlTreeIterator operator++(int);
	RtlAvlTreeIterator operator--(int);
	bool operator==(const RtlAvlTreeIterator& x) const noexcept;
};
}


class RtlAvlTree : public KObject
{
public:
	using value_type = RtlBalanceNode;
	using iterator = details::RtlAvlTreeIterator;
	using const_iterator = details::RtlAvlTreeConstIterator;

	RtlAvlTree(MmVirtualAddress vaddr);
	RtlAvlTree() = default;
	~RtlAvlTree() = default;

	RtlBalanceNode Root() const;

	const_iterator begin() const;
	const_iterator end() const;
	iterator begin();
	iterator end();

private:
	RtlBalanceNode BegNode() const;
	RtlBalanceNode EndNode() const;
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
	using KObject::KObject;

	ObjectT Object() {
		return vaddr_.Value<uint64_t>() & ~0xFull;
	}

	uint64_t RefCnt() {
		return vaddr_.Value<uint64_t>() & 0xFull;
	}
};
}