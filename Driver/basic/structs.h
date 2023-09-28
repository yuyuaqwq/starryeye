#pragma once

#include <optional>
#include "virtual_addr.h"
#include <krnlib/functional.hpp>

namespace stareye {
class KObject {
public:
	KObject() = default;
	KObject(const MmVirtualAddress& vaddr);
	const MmVirtualAddress& VAddr() const;

protected:
	MmVirtualAddress vaddr_;
};

template<class T>
using EnableIfInheritKObject = typename std::enable_if_t<std::is_convertible_v<T*, KObject*>, int>;

class RtlBalanceNode: public KObject
{
public:
	RtlBalanceNode(const MmVirtualAddress& vaddr);
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

	template<class T, std::enable_if_t<std::is_convertible_v<T*, RtlBalanceNode*>, int> = 0>
	T Impl() {
		return vaddr_;
	}
};

class RtlAvlTree : public KObject
{
public:
	class Iterator
	{
	public: 
		using difference_type = std::ptrdiff_t;
		using value_type = RtlBalanceNode;
		using pointer = value_type*;
		using reference = value_type&;
		using iterator_category = std::bidirectional_iterator_tag;

		Iterator(const RtlBalanceNode& cur);
		~Iterator() = default;
		RtlBalanceNode& operator*();

		Iterator& operator++();
		Iterator& operator--();

		Iterator operator++(int);
		Iterator operator--(int);

	private:
		void GoLeftMost(const RtlBalanceNode& node);
		void GoRightMost(const RtlBalanceNode& node);

		RtlBalanceNode cur_;
	};

	using ForeachCallBackT = const krnlib::function<bool(const RtlBalanceNode&)>&;

	RtlAvlTree(const MmVirtualAddress& vaddr);
	RtlAvlTree() = default;
	~RtlAvlTree() = default;

	RtlBalanceNode Root();
	void Foreach(ForeachCallBackT callback);
	// 获取所有节点(性能差, 不推荐使用!!!)
	krnlib::list<RtlBalanceNode> GetAllNodes();

private:
	bool ForeachRecursion(const RtlBalanceNode& root, ForeachCallBackT callback);
};



template<class T>
class ListEntry : public KObject
{
public:
	ListEntry() = default;
	template<EnableIfInheritKObject<T> = 0>
	ListEntry(const MmVirtualAddress& vaddr, ptrdiff_t off_to_head): KObject(vaddr), off_to_head_(off_to_head) {}
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
	template<EnableIfInheritKObject<ObjectT> = 0>
	ExFastRef(const MmVirtualAddress& vaddr) : KObject(vaddr) {}
	~ExFastRef() = default;

	ObjectT Object() {
		return vaddr_.Value<uint64_t>() & ~0xFull;
	}

	uint64_t RefCnt() {
		return vaddr_.Value<uint64_t>() & 0xFull;
	}
};
}