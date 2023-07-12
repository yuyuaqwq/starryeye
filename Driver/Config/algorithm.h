#pragma once
#include <ntifs.h>
#include "Config/base.h"
#include "Config/yfstd.h"

namespace StarryEye {
template<class KObjT>
class KObjListEntry: public KObjectBase
{
public:
	KObjListEntry(ULONG64 list_addr, ULONG64 offset): KObjectBase(list_addr)
	{
		list_ = (PLIST_ENTRY64)list_addr;
		offset_ = offset;
	}
	KObjListEntry(std::nullptr_t): KObjectBase(nullptr)
	~KObjListEntry() {}

	KObjListEntry Flink()
	{
		return KObjListEntry((ULONG64)list_->Flink, offset_);
	}
	KObjListEntry Blink()
	{
		return KObjListEntry((ULONG64)list_->Blink, offset_);
	}

	KObjT Object()
	{
		return KObjT((ULONG64)list_ - offset_);
	}

private:
	PLIST_ENTRY64 list_;
	ULONG64 offset_;
};

template<class DataT>
class RtlBalanceNode: public KObjectBase
{
public:
	RtlBalanceNode(ULONG64 address): KObjectBase(address), data_(address) {}
	RtlBalanceNode(std::nullptr_t): KObjectBase(nullptr), data_(nullptr) {}
	~RtlBalanceNode() {}

	RtlBalanceNode Left()
	{
		return RtlBalanceNode(*(PULONG64)(address_ + AlogrithmOffsets::RtlBalanceNode_Left));
	}
	RtlBalanceNode Right()
	{
		return RtlBalanceNode(*(PULONG64)(address_ + AlogrithmOffsets::RtlBalanceNode_Right));
	}
	RtlBalanceNode ParentValue()
	{
		return RtlBalanceNode(*(PULONG64)(address_ + AlogrithmOffsets::RtlBalanceNode_ParentValue));
	}
	DataT* operator->() {
		return &data_;
	}

private:
	DataT data_;
};

template<class DataT>
class RtlAvlTree: public KObjectBase
{
public:
	using NodeT = RtlBalanceNode<DataT>;
	using ForeachCallBackT = const yfstd::function<void(NodeT&)>&;

	RtlAvlTree(ULONG64 address): KObjectBase(address) {}
	RtlAvlTree(std::nullptr_t): KObjectBase(nullptr) {}
	~RtlAvlTree() {}

	NodeT Root() {
		return NodeT(*(PULONG64)(address_ + AlogrithmOffsets::RtlAvlTree_RootOffset));
	}

	void Foreach(ForeachCallBackT callback) {
		ForeachRecursion(Root(), callback);
	}

	// 获取所有节点(性能差, 不推荐使用!!!)
	yfstd::list<NodeT> GetAllNodes() {
		yfstd::list<NodeT> total;
		Foreach([&](NodeT& node) {
			total.push_back(node);
			});
		return total;
	}

private:
	void ForeachRecursion(NodeT& root, ForeachCallBackT callback)
	{
		if (!root.IsVaild()) return;
		ForeachRecursion(root.Left(), callback);
		callback(root);
		ForeachRecursion(root.Right(), callback);
	}
};


class AlogrithmOffsets
{
public:
	inline static ULONG64 RtlBalanceNode_Left;
	inline static ULONG64 RtlBalanceNode_Right;
	inline static ULONG64 RtlBalanceNode_ParentValue;
	inline static ULONG64 RtlAvlTree_RootOffset;

	static void Init()
	{
		RtlBalanceNode_Left = 0;
		RtlBalanceNode_Right = 8;
		RtlBalanceNode_ParentValue = 0x10;
		RtlAvlTree_RootOffset = 0;
	}
};
}