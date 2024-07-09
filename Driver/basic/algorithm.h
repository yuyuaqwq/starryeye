#pragma once
#include <ntifs.h>
#include <stdint.h>
#include <type_traits>
#include <concepts>

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

namespace details {
	template<size_t bytes>
	constexpr int AlignBytes() {
		static_assert(bytes < 8, "bytes must be less than 8!");
		static_assert(bytes != 0, "bytes must be greater than 0!");
		if constexpr (bytes <= 2)
			return bytes;
		else if constexpr (bytes <= 4)
			return 4;
		else return 8;
	}
	template<bool is_unsigned, size_t bytes>
	struct BytesToType;
	template<> struct BytesToType<true, 1> { using Type = uint8_t; };
	template<> struct BytesToType<false, 1> { using Type = int8_t; };
	template<> struct BytesToType<true, 2> { using Type = uint16_t; };
	template<> struct BytesToType<false, 2> { using Type = int16_t; };
	template<> struct BytesToType<true, 4> { using Type = uint32_t; };
	template<> struct BytesToType<false, 4> { using Type = int32_t; };
	template<> struct BytesToType<true, 8> { using Type = uint64_t; };
	template<> struct BytesToType<false, 8> { using Type = int64_t; };
	template<size_t bytes, typename T>
	using BytesToTypeT = typename BytesToType<std::is_unsigned_v<T>, AlignBytes<bytes>()>::Type;
}

template<size_t bytes, std::integral T>
	requires (bytes <= sizeof(T))
void SetIntegerHigh(T& target, details::BytesToTypeT<bytes, T> value) {
	auto dest = (char*)&target + (sizeof(T) - bytes);
	memcpy(dest, &value, bytes);
}
}