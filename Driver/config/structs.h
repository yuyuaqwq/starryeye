#pragma once

#include "config/virtual_addr.h"
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


class RtlBalanceNode: public KObject
{
public:
	RtlBalanceNode(const MmVirtualAddress& vaddr);
	~RtlBalanceNode() = default;

	RtlBalanceNode Left() const;
	RtlBalanceNode Right() const;
	RtlBalanceNode Parent() const;
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
	// ��ȡ���нڵ�(���ܲ�, ���Ƽ�ʹ��!!!)
	krnlib::list<RtlBalanceNode> GetAllNodes();

private:
	bool ForeachRecursion(const RtlBalanceNode& root, ForeachCallBackT callback);
};
}