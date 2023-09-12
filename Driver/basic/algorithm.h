#pragma once
#include <ntifs.h>
#include <stdint.h>
#include <fustd/generic/algorithm.hpp>

namespace StarryEye {

#define HIGH_UINT32(x) (reinterpret_cast<uint16_t*>(&x)[1])
#define SET_HIGH_ULONG64(x, val) reinterpret_cast<uint32_t*>(&x)[1] = val

constexpr bool IsBitAreaValid(size_t buf_size, size_t bit_pos, size_t bit_size);

class ProcessAutoAttacker {
public:
	ProcessAutoAttacker(PEPROCESS proc);
	~ProcessAutoAttacker();
private:
	PEPROCESS proc_;
};
}