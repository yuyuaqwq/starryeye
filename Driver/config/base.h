#pragma once
#include <ntifs.h>
#include <stdint.h>

#define MAXPHYADDR 51

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