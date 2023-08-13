#pragma once
#include <ntifs.h>
#include <stdint.h>

namespace StarryEye {
// 所有Kernel Object的基类, 主要是为了提供一个IsVaild函数和nullptr构造函数
class KObjectBase
{
public:
	KObjectBase(uint64_t address);
	KObjectBase(std::nullptr_t);
	~KObjectBase();

	bool IsVaild();

	uint64_t Address();

protected:
	uint64_t address_;
};
}