#include "structs.h"

namespace stareye {
	KObject::KObject(const MmVirtualAddress& vaddr): vaddr_(vaddr) {}
	const MmVirtualAddress& KObject::VAddr() const {
		return vaddr_;
	}

	RtlBalanceNode::RtlBalanceNode(const MmVirtualAddress& vaddr) : KObject(vaddr) {}
	RtlBalanceNode RtlBalanceNode::Left() const {
		return vaddr_.Pointer<RTL_BALANCED_NODE>()->Left;
	}
	RtlBalanceNode RtlBalanceNode::Right() const {
		return vaddr_.Pointer<RTL_BALANCED_NODE>()->Right;
	}
	RtlBalanceNode RtlBalanceNode::Parent() const {
		return (PRTL_BALANCED_NODE)vaddr_.Pointer<RTL_BALANCED_NODE>()->ParentValue;
	}


	RtlAvlTree::RtlAvlTree(const MmVirtualAddress& vaddr) : KObject(vaddr) {}
	RtlBalanceNode RtlAvlTree::Root() {
		return vaddr_.ValU64();
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