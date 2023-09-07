#include "utils.h"

namespace StarryEye {
MmVirtualAddress StarryEye::MemoryUtils::AllocatePage(POOL_TYPE pool_type, size_t number_of_bytes, const char tag[4], PageProtection protection)
{
	return MmVirtualAddress();
}
void MemoryUtils::SetPageProtection(const MmVirtualAddress& address, PageProtection protection)
{
	address.Pointer()[0] = address.Pointer()[0];
	bool is_executable = protection & kPageExecutable;
	bool is_readwrite = protection & kPageReadWrite;
	address.GetPxte()->bits.execute_disable = is_executable ? 0 : 1;
	address.GetPpte()->bits.execute_disable = is_executable ? 0 : 1;
	address.GetPdte()->bits.execute_disable = is_executable ? 0 : 1;
	address.GetPte().Hand().SetExecuteDisable(!is_executable);
	address.GetPxte()->bits.read_write = is_readwrite ? 1 : 0;
	address.GetPpte()->bits.read_write = is_readwrite ? 1 : 0;
	address.GetPdte()->bits.read_write = is_readwrite ? 1 : 0;
	address.GetPte().Hand().SetExecuteDisable(!is_executable);
}
}