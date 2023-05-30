#ifndef KPDB_H_
#define KPDB_H_

#include <memory>
#include <vector>
#include <string>
#include <map>

#include <ntifs.h>
#include <ntddk.h>
#include <ntimage.h>
#include <minwindef.h>
#include <intrin.h>
#include <ntddndis.h>
#include <strsafe.h>
#include <fltkernel.h>

#ifndef _SCN
#define _SCN std::
#endif

namespace kpdb {

class Pdb {
private:
	inline static const char kMagic[32] = {
		0x4D, 0x69, 0x63, 0x72, 0x6F, 0x73, 0x6F, 0x66, 0x74, 0x20, 0x43, 0x2F,
		0x43, 0x2B, 0x2B, 0x20, 0x4D, 0x53, 0x46, 0x20, 0x37, 0x2E, 0x30, 0x30,
		0x0D, 0x0A, 0x1A, 0x44, 0x53, 0x00, 0x00, 0x00
	};

#pragma pack(push, 1)
	struct StreamData {
		_SCN vector<uint8_t> StreamPointer;
		SIZE_T StreamSize;
	};

	struct SuperBlock {
		CHAR  FileMagic[sizeof(kMagic)];
		DWORD BlockSize;
		DWORD FreeBlockMapBlock;
		DWORD NumBlocks;
		DWORD NumDirectoryBytes;
		DWORD Unknown;
		DWORD BlockMapAddr;
	};

	struct DBIHeader {
		LONG	VersionSignature;
		DWORD	VersionHeader;
		DWORD	Age;
		WORD	GlobalStreamIndex;
		WORD	BuildNumber;
		WORD	PublicStreamIndex;
		WORD	PdbDllVersion;
		WORD	SymRecordStream;
		WORD	PdbDllRbld;
		LONG	ModInfoSize;
		LONG	SectionContributionSize;
		LONG	SectionMapSize;
		LONG	SourceInfoSize;
		LONG	TypeServerSize;
		DWORD	MFCTypeServerIndex;
		LONG	OptionalDbgHeaderSize;
		LONG	ECSubstreamSize;
		WORD	Flags;
		WORD	Machine;
		DWORD	Padding;
	};

	struct PUBSYM32 {
		WORD reclen;     // Record length
		WORD rectyp;     // S_PUB32
		DWORD pubsymflags;
		DWORD off;
		WORD seg;
		char name[1];    // Length-prefixed name
	};

	enum SYM_ENUM_e {
		// [...]
		S_CONSTANT = 0x1107,  // constant symbol
		S_UDT = 0x1108,  // User defined type
		S_LDATA32 = 0x110c,  // Module-local symbol
		S_GDATA32 = 0x110d,  // Global data symbol
		S_PUB32 = 0x110e, // a public symbol (CV internal reserved)
		S_PROCREF = 0x1125, // Reference to a procedure
		S_LPROCREF = 0x1127, // Local Reference to a procedure
		// [...]
	};

public:
	struct SymbolData {
		_SCN string name;
		//UINT SymbolNameHash;
		DWORD section_offset;
		DWORD symbol_rva;
		WORD section_number;
	};
#pragma pack(pop)

public:
	bool ParseSymbol(void* pdb) {
		SuperBlock* super = (SuperBlock*)pdb;
		if (!IsPdbMagicValid(super)) return false;
		DWORD block_size = super->BlockSize;

		SuperBlock* super = (SuperBlock*)pdb;
		DWORD size = super->NumDirectoryBytes;
		DWORD block_size = super->BlockSize;
		DWORD block_count = (size + block_size - 1) / block_size;
		PDWORD block_id_array = (PDWORD)((BYTE*)pdb + block_size * super->BlockMapAddr);

		_SCN vector<uint8_t> stream_dir(block_count * block_size);

		uint8_t* end_of_stream = stream_dir.data();
		for (int i = 0; i < block_count; ++i) {
			PCHAR block = (PCHAR)pdb + block_size * block_id_array[i];
			memcpy(end_of_stream, block, block_size);
			end_of_stream += block_size;
		}

		PDWORD ui32_iter = (PDWORD)stream_dir.data();
		DWORD stream_num = *ui32_iter++;
		PDWORD stream_array = ui32_iter;
		ui32_iter += stream_num;

		// ½âÎöÁ÷
		stream_data_list_.resize(stream_num);
		size_t streams_count = 0;

		for (int i = 0; i < stream_num; ++i) {
			_SCN vector<uint8_t> current_stream;
			DWORD current_stream_size = stream_array[i];
			DWORD current_stream_block_count = (current_stream_size + block_size - 1) / block_size;

			current_stream.resize(current_stream_block_count * block_size);

			uint8_t* end_of_stream = current_stream.data();
			for (int j = 0; j < current_stream_block_count; ++j) {
				DWORD block_id = *ui32_iter++;
				uint8_t* block = (uint8_t*)pdb + (block_size * block_id);

				memcpy(end_of_stream, block, block_size);
				end_of_stream += block_size;
			}

			StreamData current_stream_data;
			current_stream_data.StreamPointer = current_stream;
			current_stream_data.StreamSize = current_stream_size;
			stream_data_list_[streams_count] = current_stream_data;
			streams_count++;
		}

		// ½âÎö·ûºÅ
		uint8_t* types = stream_data_list_[2].StreamPointer.data();
		uint8_t* symbols = stream_data_list_[(((DBIHeader*)stream_data_list_[3].StreamPointer.data())->SymRecordStream)].StreamPointer.data();
		SIZE_T symbols_stream_size = stream_data_list_[
			((DBIHeader*)stream_data_list_[3].StreamPointer.data())->SymRecordStream
		].StreamSize;

		uint8_t* it = symbols;
		const uint8_t* end = (uint8_t*)((SIZE_T)it + symbols_stream_size);
		while (it != end) {
			const PUBSYM32* curr = (PUBSYM32*)it;
			if (curr->rectyp == S_PUB32) {
				DWORD iteration = 0;

				SymbolData symbol_data;
				symbol_data.name = curr->name;
				symbol_data.section_number = curr->seg;
				symbol_data.section_offset = curr->off;
						
				symbol_data_map_.insert(std::make_pair(symbol_data.name, symbol_data));

				iteration++;
			}
			it += curr->reclen + 2;
		}
	}

	SymbolData GetSymbol(const _SCN string& str) {
		if (symbol_data_map_.find(str) == symbol_data_map_.end()) {
			return SymbolData();
		}
		return symbol_data_map_[str];
	}

	void ConvertSecOffsetToRva(DWORD64 ModuleBase) {
		PIMAGE_NT_HEADERS NTHeader = (PIMAGE_NT_HEADERS)(ModuleBase + ((PIMAGE_DOS_HEADER)ModuleBase)->e_lfanew);
		PIMAGE_SECTION_HEADER SectionHeaderBaseAddr = (PIMAGE_SECTION_HEADER)((DWORD64)NTHeader + sizeof(IMAGE_NT_HEADERS));
		for(auto& symbol_data : symbol_data_map_) {
			symbol_data.second.symbol_rva =
				symbol_data.second.section_offset + SectionHeaderBaseAddr[symbol_data.second.section_number - 1].VirtualAddress;
		}
	}

private:
	bool IsPdbMagicValid(SuperBlock* super) {
		return 0 == memcmp(super->FileMagic, kMagic, sizeof(kMagic));
	}

private:
	_SCN vector<StreamData> stream_data_list_;
	_SCN map<_SCN string, SymbolData> symbol_data_map_;
};

}; // namespace kpdb


#endif // KPDB_H_