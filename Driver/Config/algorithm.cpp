#include "Config/algorithm.h"

namespace StarryEye {
ListEntry::ListEntry(ULONG64 list_addr, ULONG64 offset) :
    KObjectBase(list_addr),
    list_((PLIST_ENTRY64)list_addr),
    offset_(offset) {}
ListEntry::ListEntry(std::nullptr_t) :
    KObjectBase(nullptr),
    list_(0),
    offset_(0) {}
ListEntry::~ListEntry() {}


krnlib::Option<ULONG64> StarryEye::GetBitAreaValue(PVOID buffer, ULONG64 pos, UCHAR bits)
{
    if (bits > 64) {
        return krnlib::None();
    }

    ULONG64 value = 0;
    UCHAR* byteBuffer = static_cast<UCHAR*>(buffer);

    for (ULONG64 i = 0; i < bits; ++i) {
        ULONG64 byteIndex = (pos + i) / 8;
        ULONG64 bitIndex = (pos + i) % 8;

        ULONG64 bitValue = (byteBuffer[byteIndex] >> bitIndex) & 1;
        value |= (bitValue << i);
    }

    return krnlib::Some<ULONG64>(value);
}
}
