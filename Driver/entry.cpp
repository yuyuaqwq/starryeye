#include <ntifs.h>

#include <jxy/string.hpp>
#include <jxy/vector.hpp>
#include <jxy/memory.hpp>

#include <yuJson/json.hpp>


extern "C" NTSTATUS DriverEntry(IN PDRIVER_OBJECT pDriverObject, IN PUNICODE_STRING pRegistryPath){
	jxy::string<POOL_TYPE::NonPagedPool, 'FAKE'> str = "??";
	//std::vector<int> aaaa;

	//yuJson::Json json = { "fake", 666, "emm", true };

	KdPrint(("Enter DriverEntry!%s\n", str.c_str()));

	

	return STATUS_SUCCESS;
}
