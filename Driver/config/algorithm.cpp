#include "config/algorithm.h"

namespace StarryEye {
ListEntry::ListEntry(uint64_t list_addr, uint64_t offset) :
    KObjectBase(list_addr),
    list_((PLIST_ENTRY64)list_addr),
    offset_(offset) {}
ListEntry::ListEntry(std::nullptr_t) :
    KObjectBase(nullptr),
    list_(0),
    offset_(0) {}

ListEntry ListEntry::Flink()
{
    return ListEntry((uint64_t)list_->Flink, offset_);
}

ListEntry ListEntry::Blink()
{
    return ListEntry((uint64_t)list_->Blink, offset_);
}

fustd::Option<uint64_t> StarryEye::GetBitAreaValue(PVOID buffer, size_t buf_size, size_t pos, uint8_t bits)
{
    if (bits > 64 || pos + bits > buf_size * 8) {
        return fustd::None();
    }

    uint64_t value = 0;
    uint8_t* byteBuffer = static_cast<uint8_t*>(buffer);

    for (uint64_t i = 0; i < bits; ++i) {
        uint64_t byteIndex = (pos + i) / 8;
        uint64_t bitIndex = (pos + i) % 8;

        uint64_t bitValue = (byteBuffer[byteIndex] >> bitIndex) & 1;
        value |= (bitValue << i);
    }

    return fustd::Some(std::move(value));
}
}
