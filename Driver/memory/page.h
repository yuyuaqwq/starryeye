#pragma once
#include "config/base.h"
#include <intrin.h>

//TODO ·ÖÒ³»úÖÆ´ý²âÊÔ.....
namespace StarryEye {
namespace details {
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


union PdpteFormater
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

union Pml4Formater
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

union VirtualAddressFormater
{
    uint64_t quad_part;
    uint64_t* ptr;
    struct
    {
        uint64_t offset : 12;           // 0
        uint64_t pti : 9;               // 12
        uint64_t pdti : 9;              // 21
        uint64_t ppti : 9;             // 30
        uint64_t pxti : 9;             // 39
        uint64_t sign_extend : 16;      // 48
    } bits;
};

VirtualAddressFormater LocatePml4Base();
}

class MmPte : public KObjectBase
{
public:
    static void Init();

    class Handware : public KObjectBase
    {
    public:
        ~Handware() = default;
        bool IsVaild() override;
        uint64_t PageFrameNumber();

    private:
        friend class MmPte;

        static inline uint64_t PageFrameNumberBitPos;
        static inline uint64_t PageFrameNumberBitSize;

        Handware(details::VirtualAddressFormater pte_base);
        details::VirtualAddressFormater pte_base_;
        uint64_t pte_;
    };

    MmPte() = default;
    MmPte(details::VirtualAddressFormater pte_base);
    ~MmPte() = default;

    Handware Hand();

private:
    details::VirtualAddressFormater pte_base_;
};


class MmPt : public KObjectBase
{
public:
    MmPt() = default;
    MmPt(details::VirtualAddressFormater pt_base);
    ~MmPt() = default;

    MmPte operator[](size_t pti);

private:
    details::VirtualAddressFormater pt_base_;
};


class MmPdt : public KObjectBase
{
public:
    MmPdt() = default;
    MmPdt(details::VirtualAddressFormater pdt_base);
    ~MmPdt() = default;

    MmPt operator[](size_t pdti);

private:
    details::VirtualAddressFormater pdt_base_;
};

class MmPpt : public KObjectBase
{
public:
    MmPpt() = default;
    MmPpt(details::VirtualAddressFormater ppt_base);
    ~MmPpt() = default;

    MmPdt operator[](size_t ppti);

private:
    details::VirtualAddressFormater ppt_base_;
};

class MmPxt : public KObjectBase
{
public:
    MmPxt() = default;
    MmPxt(uint64_t kproc_addr);
    ~MmPxt() = default;

    MmPpt operator[](size_t pxti);

private:
    details::VirtualAddressFormater pxt_base_;
};


class MmVirtualAddress : public KObjectBase
{
public:
	MmVirtualAddress() = default;
	MmVirtualAddress(uint64_t va, uint64_t owner_kproc_addr);
	~MmVirtualAddress() = default;

    //static MmVirtualAddress FromPhysicalAddress(uint64_t phyaddr, uint64_t kproc_addr);

    uint64_t PxtIndex();
    uint64_t PptIndex();
    uint64_t PdtIndex();
    uint64_t PtIndex();
    uint64_t Offset();

    MmPte PageProperty();

private:
	uint64_t owner_kproc_addr_;
    MmPxt directory_table_;
    details::VirtualAddressFormater vaddr_;
};
}