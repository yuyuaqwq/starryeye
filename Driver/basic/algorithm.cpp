#include "algorithm.h"

namespace stareye {
ProcessAutoAttacker::ProcessAutoAttacker(PEPROCESS proc)
    : proc_(proc) {
    if (proc)
        KeAttachProcess(proc);
}
ProcessAutoAttacker::~ProcessAutoAttacker() {
    if (proc_)
        KeDetachProcess();
}

inline constexpr bool IsBitAreaValid(size_t buf_size, size_t bit_pos, size_t bit_size) {
    return fustd::CeilDiv(bit_pos + bit_size, 8) <= bit_size;
}
bool EqualString(const char* str1, const char* str2, bool case_in_sensitive)
{
    STRING kstr1;
    RtlInitString(&kstr1, str1);
    STRING kstr2;
    RtlInitString(&kstr2, str2);
    return RtlCompareString(&kstr1, &kstr2, case_in_sensitive) == 0;
}
}
