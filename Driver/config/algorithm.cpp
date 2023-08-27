#include "config/algorithm.h"

namespace StarryEye {
ListEntry::ListEntry(uint64_t list_addr, uint64_t offset) :
    KObjectBase(list_addr),
    list_((PLIST_ENTRY64)list_addr),
    offset_(offset) {}

ListEntry ListEntry::Flink()
{
    return ListEntry((uint64_t)list_->Flink, offset_);
}

ListEntry ListEntry::Blink()
{
    return ListEntry((uint64_t)list_->Blink, offset_);
}

fustd::Option<uint64_t> StarryEye::GetBitAreaValue(PVOID buffer, size_t buf_size, size_t bit_pos, uint8_t bits)
{
    if (bits > 64 || bit_pos + bits > buf_size * 8) {
        return fustd::None();
    }

    uint64_t value = 0;
    uint8_t* byteBuffer = static_cast<uint8_t*>(buffer);

    for (uint64_t i = 0; i < bits; ++i) {
        uint64_t byteIndex = (bit_pos + i) / 8;
        uint64_t bitIndex = (bit_pos + i) % 8;

        uint64_t bitValue = (byteBuffer[byteIndex] >> bitIndex) & 1;
        value |= (bitValue << i);
    }

    return fustd::Some(std::move(value));
}
}