#pragma once
#include "config/base.h"
//TODO ·ÖÒ³»úÖÆ´ý²âÊÔ.....
namespace StarryEye {
namespace details {
    class MmPageHandware
    {
    public:
        static void Init();
        MmPageHandware(uint64_t pte);
        ~MmPageHandware() = default;

        uint64_t Valid();
        uint64_t LargePage();
        uint64_t PageFrameNumber();

    private:
        inline static uint64_t LargePageBitPos;
        inline static uint64_t LargePageBitSize;
        inline static uint64_t PageFrameNumberBitPos;
        inline static uint64_t PageFrameNumberBitSize;

        uint64_t pte_;
    };
}

class MmPage : public KObjectBase
{
public:
    MmPage(std::nullptr_t);
    MmPage(uint64_t pte);
    ~MmPage() = default;

    details::MmPageHandware Hand();

private:
    uint64_t Address() override { return 0; };
    bool IsVaild() override { return false; };
    uint64_t pte_;
};


class MmPageTable : public KObjectBase
{
public:
    MmPageTable(std::nullptr_t);
    MmPageTable(uint64_t pdte);
    ~MmPageTable() = default;

    bool Is2mPage();
    uint64_t Address() override;
    MmPage operator[](size_t idx);

private:
    union
    {
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
            uint64_t accessed : 1;              // 11
            uint64_t pdt_align : 24;            // 12
            uint64_t reserved : 16;             // 36
            uint64_t ignored3 : 11;             // 52
            uint64_t execute_disable : 1;       // 63
        } pdte_;
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
            uint64_t page_align : 15;          // 21
            uint64_t reserved2 : 6;            // 36
            uint64_t ignored : 7;              // 52
            uint64_t protection_key : 4;       // 59
            uint64_t execute_disable : 1;      // 63
        } pdte_2m_;
        uint64_t ulonglong_;
    };
};

class MmPageDirectoryTable : public KObjectBase
{
public:
    MmPageDirectoryTable(std::nullptr_t);
    MmPageDirectoryTable(uint64_t pdpte);
    ~MmPageDirectoryTable() = default;

    bool Is1gPage();
    uint64_t Address() override;
    MmPageTable operator[](size_t idx);

private:
    union
    {
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
            uint64_t pdpt_align : 24;          // 12
            uint64_t reserved : 16;            // 36
            uint64_t ignored3 : 11;            // 52
            uint64_t execute_disable : 1;      // 63
        } pdpte_;
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
            uint64_t page_align : 6;           // 30
            uint64_t reserved2 : 16;           // 36
            uint64_t ignored : 7;              // 52
            uint64_t protection_key : 4;       // 59
            uint64_t execute_disable : 1;      // 63
        } pdpte_1g_;
        uint64_t ulonglong_;
    };
};

class MmPageDirectoryPointerTable : public KObjectBase
{
public:
    MmPageDirectoryPointerTable(std::nullptr_t);
    MmPageDirectoryPointerTable(uint64_t pml4e);
    ~MmPageDirectoryPointerTable() = default;

    uint64_t Address() override;
    MmPageDirectoryTable operator[](size_t idx);

private:
    union
    {
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
            uint64_t pdpt_align : 24;       // 12
            uint64_t reserved2 : 16;        // 36
            uint64_t ignored3 : 11;         // 52
            uint64_t execute_disable : 1;   // 63
        }pml4e_;
        uint64_t ulonglong_;
    };
};

class MmPageMapLevel4 : public KObjectBase
{
public:
    MmPageMapLevel4(std::nullptr_t);
    MmPageMapLevel4(uint64_t cr3);
    ~MmPageMapLevel4() = default;

    uint64_t Address() override;
    MmPageDirectoryPointerTable operator[](size_t idx);

private:
    union
    {
        struct
        {
            uint64_t ignored : 3;           // 0
            uint64_t write_through : 1;     // 3
            uint64_t cache_disable : 1;     // 4
            uint64_t ignored2 : 7;          // 5
            uint64_t pml4_align : 24;       // 12
            uint64_t reserved : 28;         // 36
        } cr3_;
        uint64_t ulonglong_;
    };
};

class MmVirtualAddress : public KObjectBase
{
public:
	MmVirtualAddress(std::nullptr_t);
	MmVirtualAddress(uint64_t va, uint64_t owner_kproc_addr);
	~MmVirtualAddress() = default;

    uint64_t Pml4Index();
    uint64_t PdptIndex();
    uint64_t PdtIndex();
    uint64_t PtIndex();
    uint64_t Offset();

    uint64_t ToPhysicalAddress();

private:
	uint64_t owner_kproc_addr_;
    MmPageMapLevel4 directory_table_;
    union {
        struct {
            uint64_t offset : 12;           // 0
            uint64_t pt_idx : 9;            // 12
            uint64_t pdt_idx : 9;           // 21
            uint64_t pdpt_idx : 9;          // 30
            uint64_t pml4_idx : 9;          // 39
            uint64_t sign_extend : 16;      // 48
        } vaddr_bits;
        uint64_t vaddr_;
    };
};
}