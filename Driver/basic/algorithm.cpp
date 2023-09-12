#include "algorithm.h"

namespace StarryEye {
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
}
