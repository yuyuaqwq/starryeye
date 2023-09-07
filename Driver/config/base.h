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

	virtual bool IsVaild() const;

	virtual uint64_t Address() const;
	template<class T = char>
	T* Pointer() const;

protected:
	uint64_t address_;
};

template<class T>
inline T* KObjectBase::Pointer() const
{
	return (T*)Address();
}
}