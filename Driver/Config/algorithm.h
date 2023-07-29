#pragma once
#include <ntifs.h>
#include "Config/base.h"
#include <krnlib/functional.hpp>
#include <krnlib/stl_container.hpp>
#include <krnlib/option.hpp>

namespace StarryEye {

#define HIGH_UINT32(x) (reinterpret_cast<PUINT16>(&x)[1])
#define SET_HIGH_ULONG64(x, val) reinterpret_cast<PUINT32>(&x)[1] = val

class ListEntry: public KObjectBase
{
public:
	ListEntry(ULONG64 list_addr, ULONG64 offset);
	ListEntry(std::nullptr_t);
	~ListEntry();

	ListEntry Flink()
	{
		return ListEntry((ULONG64)list_->Flink, offset_);
	}
	ListEntry Blink()
	{
		return ListEntry((ULONG64)list_->Blink, offset_);
	}

	template<class KObjT>
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
	using ForeachCallBackT = const krnlib::function<bool(NodeT&)>&;

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
	krnlib::list<NodeT> GetAllNodes() {
		krnlib::list<NodeT> total;
		Foreach([&](NodeT& node) {
			total.push_back(node);
			return true;
			});
		return total;
	}

private:
	bool ForeachRecursion(NodeT& root, ForeachCallBackT callback)
	{
		if (!root.IsVaild()) return true;
		if (!ForeachRecursion(root.Left(), callback)) return false;
		if (!callback(root)) return false;
		return ForeachRecursion(root.Right(), callback);
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

krnlib::Option<ULONG64> GetBitAreaValue(PVOID buffer, ULONG64 pos, UCHAR bits);
}