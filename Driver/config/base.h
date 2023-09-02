#pragma once
#include <ntifs.h>
#include <stdint.h>

#define MAXPHYADDR 51

namespace StarryEye {
// 所有Kernel Object的基类, 主要是为了提供一个IsVaild函数
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