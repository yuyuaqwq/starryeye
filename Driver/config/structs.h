#pragma once

#include "config/virtual_addr.h"

namespace StarryEye {
template<class DataT>
class RtlAvlTree
{
public:
	using ForeachCallBackT = const krnlib::function<bool(NodeT&)>&;

	RtlAvlTree(uint64_t address) : KObjectBase(address) {}
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
}