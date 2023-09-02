#pragma once
#include "config/base.h"
#include <intrin.h>

//TODO ·ÖÒ³»úÖÆ´ý²âÊÔ.....
namespace StarryEye {
namespace details {
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

#define VIRTUAL_ADDRESS_OFFSET(address) (reinterpret_cast<details::VirtualAddressFormater*>(&address)->bits.offset)
#define VIRTUAL_ADDRESS_PTI(address) (reinterpret_cast<details::VirtualAddressFormater*>(&address)->bits.pti)
#define VIRTUAL_ADDRESS_PDTI(address) (reinterpret_cast<details::VirtualAddressFormater*>(&address)->bits.pdti)
#define VIRTUAL_ADDRESS_PPTI(address) (reinterpret_cast<details::VirtualAddressFormater*>(&address)->bits.ppti)
#define VIRTUAL_ADDRESS_PXTI(address) (reinterpret_cast<details::VirtualAddressFormater*>(&address)->bits.pxti)
#define VALID_VIRTUAL_ADDRESS_MASK 0x7FFFFFFFF8ull

VirtualAddressFormater LocatePxtBase();
VirtualAddressFormater LocatePteBase();
VirtualAddressFormater GetPteVirtualAddress(uint64_t address);
VirtualAddressFormater GetPdteVirtualAddress(uint64_t address);
VirtualAddressFormater GetPpteVirtualAddress(uint64_t address);
VirtualAddressFormater GetPxteVirtualAddress(uint64_t address);
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

        Handware(uint64_t address);
    };

    MmPte() = default;
    MmPte(uint64_t address);
    ~MmPte() = default;

    Handware Hand();
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

    MmPte GetPte();
    PdteFormater* GetPdte();
    PpteFormater* GetPpte();
    PxteFormater* GetPxte();

private:
	uint64_t owner_kproc_addr_;
};
}