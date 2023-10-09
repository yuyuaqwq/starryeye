#pragma once
#include "algorithm.h"
#include <stdint.h>
#include <intrin.h>
#include <vector>
#include <krnfmt/format.h>
#include <krnlib/iostream.hpp>

//TODO 分页机制待测试.....
namespace stareye {
class MmPte;

namespace details {
union VirtualAddressFormater
{
    uint64_t quad_part;
    uint64_t* ptr;
    struct
    {
        uint64_t offset : 12;          // 0
        uint64_t pti : 9;              // 12
        uint64_t pdti : 9;             // 21
        uint64_t ppti : 9;             // 30
        uint64_t pxti : 9;             // 39
        uint64_t sign_extend : 16;     // 48
    } bits;
};

#define VIRTUAL_ADDRESS_OFFSET(vaddr) (reinterpret_cast<const details::VirtualAddressFormater*>(&vaddr)->bits.offset)
#define VIRTUAL_ADDRESS_PTI(vaddr) (reinterpret_cast<const details::VirtualAddressFormater*>(&vaddr)->bits.pti)
#define VIRTUAL_ADDRESS_PDTI(vaddr) (reinterpret_cast<const details::VirtualAddressFormater*>(&vaddr)->bits.pdti)
#define VIRTUAL_ADDRESS_PPTI(vaddr) (reinterpret_cast<const details::VirtualAddressFormater*>(&vaddr)->bits.ppti)
#define VIRTUAL_ADDRESS_PXTI(vaddr) (reinterpret_cast<const details::VirtualAddressFormater*>(&vaddr)->bits.pxti)
#define VALID_VIRTUAL_ADDRESS_MASK 0x7FFFFFFFF8ull

VirtualAddressFormater LocatePxtBase();
VirtualAddressFormater LocatePteBase();
VirtualAddressFormater GetPteVirtualAddress(uint64_t vaddr);
VirtualAddressFormater GetPdteVirtualAddress(uint64_t vaddr);
VirtualAddressFormater GetPpteVirtualAddress(uint64_t vaddr);
VirtualAddressFormater GetPxteVirtualAddress(uint64_t vaddr);
}

union PdteFormater
{
uint64_t quad_part;
struct
{
    uint64_t valid : 1;                 // 0
    uint64_t read_write : 1;            // 1
    uint64_t user_supervisor : 1;       // 2
    uint64_t write_through : 1;         // 3
    uint64_t cache_disable : 1;         // 4
    uint64_t accessed : 1;              // 5
    uint64_t ignored : 1;               // 6
    uint64_t large_page : 1;            // 7
    uint64_t ignored2 : 3;              // 8
    uint64_t restart : 1;               // 11
    uint64_t pdt_align : 40;            // 12
    uint64_t ignored3 : 11;             // 52
    uint64_t execute_disable : 1;       // 63
} bits;
struct
{
    uint64_t valid : 1;                // 0
    uint64_t read_write : 1;           // 1
    uint64_t user_supervisor : 1;      // 2
    uint64_t write_through : 1;        // 3
    uint64_t cache_disable : 1;        // 4
    uint64_t accessed : 1;             // 5
    uint64_t dirty : 1;                // 6
    uint64_t large_page : 1;           // 7
    uint64_t global : 1;               // 8
    uint64_t ignored2 : 2;             // 9
    uint64_t restart : 1;              // 11
    uint64_t pat : 1;                  // 12
    uint64_t reserved : 8;             // 13
    uint64_t page_align : 31;          // 21
    uint64_t ignored : 7;              // 52
    uint64_t protection_key : 4;       // 59
    uint64_t execute_disable : 1;      // 63
} bits_2m;
};


union PpteFormater
{
    uint64_t quad_part;
    struct
    {
        uint64_t valid : 1;                // 0
        uint64_t read_write : 1;           // 1
        uint64_t user_supervisor : 1;      // 2
        uint64_t write_through : 1;        // 3
        uint64_t cache_disable : 1;        // 4
        uint64_t accessed : 1;             // 5
        uint64_t ignored : 1;              // 6
        uint64_t large_page : 1;           // 7
        uint64_t ignored2 : 3;             // 8
        uint64_t restart : 1;              // 11
        uint64_t pdpt_align : 40;          // 12
        uint64_t ignored3 : 11;            // 52
        uint64_t execute_disable : 1;      // 63
    } bits;
    struct
    {
        uint64_t valid : 1;                // 0
        uint64_t read_write : 1;           // 1
        uint64_t user_supervisor : 1;      // 2
        uint64_t write_through : 1;        // 3
        uint64_t cache_disable : 1;        // 4
        uint64_t accessed : 1;             // 5
        uint64_t dirty : 1;                // 6
        uint64_t large_page : 1;           // 7
        uint64_t global : 1;               // 8
        uint64_t ignored2 : 2;             // 9
        uint64_t restart : 1;              // 11
        uint64_t pat : 1;                  // 12
        uint64_t reserved : 17;            // 13
        uint64_t page_align : 22;          // 30
        uint64_t ignored : 7;              // 52
        uint64_t protection_key : 4;       // 59
        uint64_t execute_disable : 1;      // 63
    } bits_1g;
};

union PxteFormater
{
    uint64_t quad_part;
    struct
    {
        uint64_t valid : 1;             // 0
        uint64_t read_write : 1;        // 1
        uint64_t user_supervisor : 1;   // 2
        uint64_t write_through : 1;     // 3
        uint64_t cache_disable : 1;     // 4
        uint64_t accessed : 1;          // 5
        uint64_t ignored : 1;           // 6
        uint64_t reserved : 1;          // 7
        uint64_t ignored2 : 3;          // 8
        uint64_t restart : 1;           // 11
        uint64_t pdpt_align : 40;       // 12
        uint64_t ignored3 : 11;         // 52
        uint64_t execute_disable : 1;   // 63
    }bits;
};

union Cr3Formater
{
    uint64_t quad_part;
    struct
    {
        uint64_t ignored : 3;           // 0
        uint64_t write_through : 1;     // 3
        uint64_t cache_disable : 1;     // 4
        uint64_t ignored2 : 7;          // 5
        uint64_t pml4_align : 40;       // 12
        uint64_t reserved : 12;         // 52
    } bits;
};


enum PageProtection
{
    kPageExecutable = 0b001,
    kPageWriteable = 0b010,
    kPageCopyOnWrite = 0b100,
};


class MmVirtualAddress
{
public:
    static void Init();

    MmVirtualAddress() noexcept = default;
    MmVirtualAddress(uint64_t vaddr, PEPROCESS owner = nullptr) noexcept;
    MmVirtualAddress(void* ptr, PEPROCESS owner = nullptr) noexcept;
    ~MmVirtualAddress() noexcept = default;

    uint64_t PxtIndex() const noexcept;
    uint64_t PptIndex() const noexcept;
    uint64_t PdtIndex() const noexcept;
    uint64_t PtIndex() const noexcept;
    uint64_t PteOffset() const noexcept;

    MmPte GetPte() const;
    PdteFormater* GetPdte() const;
    PpteFormater* GetPpte() const;
    PxteFormater* GetPxte() const;

    bool IsValid() const;

    template<class T = char>
    T* Pointer() const {
        ThrowIfInvalid("无法将无效地址转换为指针: ");
        return (T*)vaddr_;
    }
    template <class T>
    MmVirtualAddress Offset(size_t off) const noexcept {
        return operator+(off * sizeof(T));
    }
    uint8_t* PtrU8() const;
    uint16_t* PtrU16() const;
    uint32_t* PtrU32() const;
    uint64_t* PtrU64() const;
    uint64_t Address() const;
    std::vector<char> Buffer(size_t size) const;
    template<class T>
    T& Value() const;
    uint64_t ValU64() const;
    uint32_t ValU32() const;
    uint16_t ValU16() const;
    uint8_t ValU8() const;
    uint64_t BitArea(size_t bit_pos, uint8_t bit_size) const;
    int Protection() const;
    PEPROCESS Owner() const;

    void WriteBuffer(size_t pos, void* buffer, size_t buf_size) const;
    void WriteBitArea(size_t beg_bit_pos, uint64_t src_value, size_t src_bit_size) const;
    bool SetProtection(int protection) const;

    void SetOwner(PEPROCESS eproc) noexcept;

    friend bool operator==(MmVirtualAddress x, MmVirtualAddress y) noexcept;
    friend bool operator!=(MmVirtualAddress x, MmVirtualAddress y) noexcept;
    friend bool operator>(MmVirtualAddress x, MmVirtualAddress y) noexcept;
    friend bool operator<(MmVirtualAddress x, MmVirtualAddress y) noexcept;
    friend bool operator>=(MmVirtualAddress x, MmVirtualAddress y) noexcept;
    friend bool operator<=(MmVirtualAddress x, MmVirtualAddress y) noexcept;
    friend MmVirtualAddress operator+(ptrdiff_t offset, MmVirtualAddress next) noexcept;

    MmVirtualAddress operator+(ptrdiff_t offset) const noexcept;
    MmVirtualAddress operator-(ptrdiff_t offset) const noexcept;

    MmVirtualAddress& operator+=(ptrdiff_t offset) noexcept;
    MmVirtualAddress& operator-=(ptrdiff_t offset) noexcept;

private:
    void* GetPteUnsafe() const;
    void* GetPdteUnsafe() const;
    void* GetPpteUnsafe() const;
    void* GetPxteUnsafe() const;

    void ThrowIfReadInvalid() const;
    void ThrowIfWriteInvalid() const;
    void ThrowIfInvalid(const char* format) const;

    uint64_t vaddr_;
    PEPROCESS owner_;
};


class MmPte
{
public:
    static void Init();

    class Handware
    {
    public:
        ~Handware() = default;
        bool Vaild() const;
        uint64_t PageFrameNumber() const;
        char NoExecute();
        void SetNoExecute(bool no_exec);
        char Write();
        void SetWrite(bool writable);
        char CopyOnWrite();
        void SetCopyOnWrite(bool copy_on_writable);

    private:
        friend class MmPte;

        static inline uint64_t PageFrameNumberBitPos;
        static inline uint64_t PageFrameNumberBitSize;
        static inline uint64_t CopyOnWriteBitPos;
        static inline uint64_t CopyOnWriteBitSize;
        static inline uint64_t WriteBitPos;
        static inline uint64_t WriteBitSize;

        Handware(MmPte* parent);

        MmPte* parent_;
    };
    MmPte() = default;
    MmPte(MmVirtualAddress pte_vaddr);
    ~MmPte() = default;

    Handware Hand();

private:
    MmVirtualAddress pte_vaddr_;
};
template<class T>
inline T& MmVirtualAddress::Value() const {
    
    ProcessAutoAttacker pa{ owner_ };
    return *Pointer<T>();
}
}

template<>
struct fmt::formatter<stareye::MmVirtualAddress> {
    std::string fmt = "{}";

    constexpr auto parse(fmt::format_parse_context& ctx) {
        auto it = ctx.begin();
        while (it != ctx.end() && *it != '}') {
            ++it;
        }
        fmt = std::string(ctx.begin() - 2, it + 1);
        return it;
    }

    template <typename FormatContext>
    auto format(const stareye::MmVirtualAddress& addr, FormatContext& ctx) {
        auto a = addr.Address();
        return fmt::vformat_to(ctx.out(), fmt, fmt::make_format_args(a));
    }
};