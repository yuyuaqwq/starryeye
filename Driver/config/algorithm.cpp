#include "config/algorithm.h"

namespace StarryEye {
ListEntry::ListEntry(uint64_t list_addr, uint64_t offset) :
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

fustd::Option<uint64_t> StarryEye::GetBitAreaValue(void* buffer, size_t buf_byte_size, size_t bit_pos, uint8_t bit_size)
{
    if (bit_size > 64 || bit_pos + bit_size > buf_byte_size * 8) {
        return fustd::None();
    }

    uint64_t value = 0;
    uint8_t* bytes_buf = static_cast<uint8_t*>(buffer);

    for (uint64_t i = 0; i < bit_size; ++i) {
        uint64_t byte_idx = (bit_pos + i) / 8;
        uint64_t bit_idx = (bit_pos + i) % 8;

        uint64_t bit_val = (bytes_buf[byte_idx] >> bit_idx) & 1;
        value |= (bit_val << i);
    }

    return fustd::Some(std::move(value));
}

bool SetBitAreaValue(void* dest_buf, size_t dest_byte_size, size_t beg_bit_pos, uint64_t src_value, size_t src_bit_size)
{
    if (src_bit_size > 64 || beg_bit_pos + src_bit_size > dest_byte_size * 8) {
        return false;
    }

    uint8_t* bytes_buf = static_cast<uint8_t*>(dest_buf);

    for (size_t i = 0; i < src_bit_size; ++i) {
        size_t byte_idx = (beg_bit_pos + i) / 8;
        size_t bit_idx = (beg_bit_pos + i) % 8;

        uint64_t bit_val = (src_value >> i) & 1;

        bytes_buf[byte_idx] &= ~(1 << bit_idx);
        bytes_buf[byte_idx] |= (bit_val << bit_idx);
    }

    return true;
}
ProcessAutoAttacker::ProcessAutoAttacker(PEPROCESS proc)
    : proc_(proc) {
    KeAttachProcess(proc);
}
ProcessAutoAttacker::~ProcessAutoAttacker() {
    KeDetachProcess();
}
}
