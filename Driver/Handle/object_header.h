#pragma once
#include "Config/base.h"
#include "Handle/object_type.h"
#include <krnlib/option.hpp>

namespace StarryEye {
class EProcess;

class ObjectHeader: public KObjectBase
{
public:
	inline static PUCHAR ObHeaderCookie;

	static void Init();
	static ULONG64 GetBodyOffset();

	// ����TypeIndex
	static UCHAR DecryptTypeIndex(ULONG64 obj_addr, UCHAR type_index);

	ObjectHeader(std::nullptr_t);
	ObjectHeader(ULONG64 address);
	~ObjectHeader();

	// ��ȡTypeIndex
	UCHAR TypeIndex();

	// ��ȡ���ܺ��TypeIndex
	UCHAR TypeIndexDecrypted();

	// ��ȡType����
	ObjectType Type();

	bool IsProcess();

	krnlib::Option<EProcess> ConvToEProc();

	// ��ȡBody��ת��Ϊ����
	template<class T>
	T Body();

private:
	inline static ULONG64 TypeIndexOffset;
	inline static ULONG64 BodyOffset;
};

template<class T>
inline T ObjectHeader::Body()
{
	return T(address_ + BodyOffset);
}

}