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

	virtual bool IsVaild();

	virtual uint64_t Address();

protected:
	uint64_t address_;
};

//#define KOBJECTBASE_DEFAULT_CONSTRUCT(class_name) \
//	class_name(std::nullptr_t): KObjectBase(nullptr){}  \
//	class_name(uint64_t address): KObjectBase(address){}
}