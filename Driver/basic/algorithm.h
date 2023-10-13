#pragma once
#include <ntifs.h>
#include <stdint.h>
#include <fustd/algorithm.hpp>

namespace stareye {
constexpr bool IsBitAreaValid(size_t buf_size, size_t bit_pos, size_t bit_size);

bool EqualString(const char* str1, const char* str2, bool case_in_sensitive);

class ProcessAutoAttacker {
public:
	ProcessAutoAttacker(PEPROCESS proc);
	~ProcessAutoAttacker();
private:
	PEPROCESS proc_;
};
}