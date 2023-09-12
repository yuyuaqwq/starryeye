#pragma once

#include "basic/virtual_addr.h"
#include <krnlib/functional.hpp>

namespace StarryEye {
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

	template<class T, std::enable_if_t<std::is_convertible_v<T*, RtlBalanceNode*>, int> = 0>
	T Impl() {
		return vaddr_;
	}
};

class RtlAvlTree : public KObject
{
public:
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



template<class T, EnableIfInheritKObject<T> = 0>
class ListEntry : public KObject
{
public:
	ListEntry() = default;
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

template<class ObjectT, EnableIfInheritKObject<ObjectT> = 0>
class ExFastRef : public KObject
{
public:
	ExFastRef(const MmVirtualAddress& vaddr) : KObject(vaddr) {}

	ObjectT Object() {
		return vaddr_.Value<uint64_t>() & ~0xFull;
	}

	uint64_t RefCnt() {
		return vaddr_.Value<uint64_t>() & 0xFull;
	}
};
}