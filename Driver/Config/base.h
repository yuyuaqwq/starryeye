#pragma once
#include <ntifs.h>
#include <stdint.h>

namespace StarryEye {
// ����Kernel Object�Ļ���, ��Ҫ��Ϊ���ṩһ��IsVaild������nullptr���캯��
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