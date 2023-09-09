#include "utils.h"

namespace StarryEye {
MmVirtualAddress StarryEye::MemoryUtils::AllocatePage(POOL_TYPE pool_type, size_t number_of_bytes, uint32_t tag, PageProtection protection)
{
	/* �����ڴ� */
	auto buf = ExAllocatePoolWithTag(pool_type, number_of_bytes, tag);
	/* ������� */
	MmVirtualAddress va((uint64_t)buf, number_of_bytes);
	/* ���ñ������� */
	SetPageProtection(va, protection);
	return va;
}

bool MemoryUtils::SetPageProtection(const MmVirtualAddress& address, PageProtection protection)
{
	/* ����һ�µ�ַ, ��ֹ���� */
	address.Pointer()[0] = address.Pointer()[0];
	/* �������� */
	bool is_executable = protection & kPageExecutable;
	bool is_readwrite = protection & kPageReadWrite;
	bool is_writecopy = protection & kPageCopyOnWrite;
	if (is_readwrite && is_writecopy) return false;						// ����ͬʱΪ��д��дʱ����
	/* �޸Ĳ���ִ��λ */
	address.GetPxte()->bits.execute_disable = is_executable ? 0 : 1;
	address.GetPpte()->bits.execute_disable = is_executable ? 0 : 1;
	address.GetPdte()->bits.execute_disable = is_executable ? 0 : 1;
	address.GetPte().Hand().SetNoExecute(!is_executable);
	/* �޸Ķ�дλ */
	address.GetPxte()->bits.read_write = is_readwrite ? 1 : 0;
	address.GetPpte()->bits.read_write = is_readwrite ? 1 : 0;
	address.GetPdte()->bits.read_write = is_readwrite ? 1 : 0;
	address.GetPte().Hand().SetWrite(is_readwrite);
	/* �޸�дʱ���� */
	address.GetPte().Hand().SetCopyOnWrite(is_readwrite);
}
}