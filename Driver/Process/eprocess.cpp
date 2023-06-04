#include "eprocess.h"

StarryEye::EProcess::EProcess(ULONG64 address): KObjectBase(address)
{
	image_file_name_offset_ = 0x5A8;
}

StarryEye::EProcess::EProcess(std::nullptr_t) : KObjectBase(nullptr)
{
	image_file_name_offset_ = 0;
}

StarryEye::EProcess::~EProcess()
{
}

PCHAR StarryEye::EProcess::ImageFileName()
{
	return (PCHAR)(address_ + image_file_name_offset_);
}
