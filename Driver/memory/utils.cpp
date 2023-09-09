#include "utils.h"

namespace StarryEye {
MmVirtualAddress StarryEye::MemoryUtils::AllocatePage(POOL_TYPE pool_type, size_t number_of_bytes, uint32_t tag, PageProtection protection)
{
	/* 分配内存 */
	auto buf = ExAllocatePoolWithTag(pool_type, number_of_bytes, tag);
	/* 构造对象 */
	MmVirtualAddress va((uint64_t)buf, number_of_bytes);
	/* 设置保护属性 */
	SetPageProtection(va, protection);
	return va;
}

bool MemoryUtils::SetPageProtection(const MmVirtualAddress& address, PageProtection protection)
{
	/* 激活一下地址, 防止倒换 */
	address.Pointer()[0] = address.Pointer()[0];
	/* 分离属性 */
	bool is_executable = protection & kPageExecutable;
	bool is_readwrite = protection & kPageReadWrite;
	bool is_writecopy = protection & kPageCopyOnWrite;
	if (is_readwrite && is_writecopy) return false;						// 不能同时为读写和写时复制
	/* 修改不可执行位 */
	address.GetPxte()->bits.execute_disable = is_executable ? 0 : 1;
	address.GetPpte()->bits.execute_disable = is_executable ? 0 : 1;
	address.GetPdte()->bits.execute_disable = is_executable ? 0 : 1;
	address.GetPte().Hand().SetNoExecute(!is_executable);
	/* 修改读写位 */
	address.GetPxte()->bits.read_write = is_readwrite ? 1 : 0;
	address.GetPpte()->bits.read_write = is_readwrite ? 1 : 0;
	address.GetPdte()->bits.read_write = is_readwrite ? 1 : 0;
	address.GetPte().Hand().SetWrite(is_readwrite);
	/* 修改写时复制 */
	address.GetPte().Hand().SetCopyOnWrite(is_readwrite);
}
}