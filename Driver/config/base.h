#pragma once
#include <ntifs.h>
#include <stdint.h>

#define MAXPHYADDR 51
#define PML4_SIZE (1 << 12)
#define PDPT_SIZE (1 << 12)
#define PDT_SIZE (1 << 12)
#define PT_SIZE (1 << 12)

namespace StarryEye {
// ����Kernel Object�Ļ���, ��Ҫ��Ϊ���ṩһ��IsVaild����
class KObjectBase
{
public:
	KObjectBase();
	KObjectBase(uint64_t address);
	~KObjectBase();

	virtual bool IsVaild();

	virtual uint64_t Address();

protected:
	uint64_t address_;
};
}