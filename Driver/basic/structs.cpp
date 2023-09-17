#include "structs.h"

namespace stareye {
	KObject::KObject(const MmVirtualAddress& vaddr): vaddr_(vaddr) {}
	const MmVirtualAddress& KObject::VAddr() const {
		return vaddr_;
	}

	RtlBalanceNode::RtlBalanceNode(const MmVirtualAddress& vaddr) : KObject(vaddr) {}
	RtlBalanceNode RtlBalanceNode::Left() const {
		return MmVirtualAddress(vaddr_.Pointer<RTL_BALANCED_NODE>()->Left, vaddr_.Owner());
	}
	RtlBalanceNode RtlBalanceNode::Right() const {
		return MmVirtualAddress(vaddr_.Pointer<RTL_BALANCED_NODE>()->Right, vaddr_.Owner());
	}
	RtlBalanceNode RtlBalanceNode::Parent() const {
		return MmVirtualAddress(vaddr_.Pointer<RTL_BALANCED_NODE>()->ParentValue, vaddr_.Owner());
	}


	RtlAvlTree::RtlAvlTree(const MmVirtualAddress& vaddr) : KObject(vaddr) {}
	RtlBalanceNode RtlAvlTree::Root() {
		return MmVirtualAddress(vaddr_.ValU64(), vaddr_.Owner());
	}
	void RtlAvlTree::Foreach(ForeachCallBackT callback) {
		ForeachRecursion(Root(), callback);
	}
	krnlib::list<RtlBalanceNode> RtlAvlTree::GetAllNodes() {
		krnlib::list<RtlBalanceNode> total;
		Foreach([&](const RtlBalanceNode& node) {
			total.push_back(node);
			return true;
			});
		return total;
	}
	bool RtlAvlTree::ForeachRecursion(const RtlBalanceNode& root, ForeachCallBackT callback) {
		if (!root.VAddr().IsValid()) return true;
		if (!ForeachRecursion(root.Left(), callback)) return false;
		if (!callback(root)) return false;
		return ForeachRecursion(root.Right(), callback);
	}
}