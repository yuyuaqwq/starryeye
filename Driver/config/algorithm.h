#pragma once
#include <ntifs.h>
#include "config/base.h"
#include <krnlib/functional.hpp>
#include <krnlib/stl_container.hpp>
#include <fustd/generic/option.hpp>

namespace StarryEye {

#define HIGH_UINT32(x) (reinterpret_cast<uint16_t*>(&x)[1])
#define SET_HIGH_ULONG64(x, val) reinterpret_cast<uint32_t*>(&x)[1] = val

class ListEntry: public KObjectBase
{
public:
	ListEntry(uint64_t list_addr, uint64_t offset);
	ListEntry() = default;
	~ListEntry() = default;

	ListEntry Flink();
	ListEntry Blink();

	template<class KObjT>
	KObjT Object() {
		return KObjT((uint64_t)list_ - offset_);
	}

private:
	PLIST_ENTRY64 list_;
	uint64_t offset_;
};

template<class DataT>
class RtlBalanceNode: public KObjectBase
{
public:
	RtlBalanceNode(uint64_t address): KObjectBase(address), data_(address) {}
	RtlBalanceNode() = default;
	~RtlBalanceNode() = default;

	RtlBalanceNode Left()
	{
		return RtlBalanceNode(*(uint64_t*)(address_ + AlogrithmOffsets::RtlBalanceNode_Left));
	}
	RtlBalanceNode Right()
	{
		return RtlBalanceNode(*(uint64_t*)(address_ + AlogrithmOffsets::RtlBalanceNode_Right));
	}
	RtlBalanceNode ParentValue()
	{
		return RtlBalanceNode(*(uint64_t*)(address_ + AlogrithmOffsets::RtlBalanceNode_ParentValue));
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

	RtlAvlTree(uint64_t address): KObjectBase(address) {}
	RtlAvlTree() = default;
	~RtlAvlTree() = default;

	NodeT Root() {
		return NodeT(*(uint64_t*)(address_ + AlogrithmOffsets::RtlAvlTree_RootOffset));
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
	inline static uint64_t RtlBalanceNode_Left;
	inline static uint64_t RtlBalanceNode_Right;
	inline static uint64_t RtlBalanceNode_ParentValue;
	inline static uint64_t RtlAvlTree_RootOffset;

	static void Init()
	{
		RtlBalanceNode_Left = 0;
		RtlBalanceNode_Right = 8;
		RtlBalanceNode_ParentValue = 0x10;
		RtlAvlTree_RootOffset = 0;
	}
};

fustd::Option<uint64_t> GetBitAreaValue(PVOID buffer, size_t buf_size, size_t bit_pos, uint8_t bits);
}