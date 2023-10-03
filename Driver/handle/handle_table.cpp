#include "handle_table.h"

namespace stareye {
uint64_t HandleTable::DecryptHandleAddress(uint64_t addr)
{
	return ((LONG64)addr >> 0x10) & ~0xF;
}

void HandleTable::Init()
{
	TableCodeOffset = 0x8;
	PspCidTable = 0xfffff802598fc5d0;	//TODO PspCidTable
}

HandleTable::HandleTable(MmVirtualAddress vaddr) :KObject(vaddr) {}

uint64_t HandleTable::TableCode() const
{
	return (vaddr_ + TableCodeOffset).ValU64();
}
 
uint8_t HandleTable::TableLevel() const
{
	return TableCode() & 0b11;
}

uint64_t HandleTable::TableAddress() const
{
	return TableCode() & ~0b11;
}

uint64_t HandleTable::MaxTableSize() const
{
	uint64_t total = 1;
	for (uint8_t i = 0; i < TableLevel() + 1; i++)
		total *= 512;
	return total;
}

uint64_t* HandleTable::TablePtr() const {
	static uint64_t* PtrCache{};
	if (!PtrCache)
		PtrCache = (uint64_t*)TableAddress();
	return PtrCache;
}

std::optional<ObjectHeader> HandleTable::GetHandleObject(uint64_t index) const
{
	if (index >= MaxTableSize()) return std::nullopt;

	switch (TableLevel())
	{
	case 0:
		return GetHandleObjectInLv1TableCode(TablePtr(), index);
	case 1:
		return GetHandleObjectInLv2TableCode(TablePtr(), index / 512, index % 512);
	case 2:
		return GetHandleObjectInLv3TableCode(TablePtr(), index / (512ULL * 512), (index % (512ULL * 512)) / 512, (index % (512ULL * 512)) % 512);
	default:
		return std::nullopt;
	}
}

HandleTable::iterator HandleTable::begin() const {
	switch (TableLevel())
	{
	case 0:
		for (size_t i = 0; i < kMaxCount; i++) {
			if (auto opt = GetHandleObjectIfLv1(i); opt.has_value() && opt.value().IsValid()) {
				return iterator(this, ~0, ~0, i);
			}
		}
		break;
	case 1:
		for (size_t i = 0; i < kMaxCount; i++) {
			for (size_t j = 0; j < kMaxCount; j++) {
				if (auto opt = GetHandleObjectIfLv2(i, j); opt.has_value() && opt.value().IsValid()) {
					return iterator(this, ~0, i, j);
				}
			}
		}
		break;
	case 2:
		for (size_t i = 0; i < kMaxCount; i++) {
			for (size_t j = 0; j < kMaxCount; j++) {
				for (size_t k = 0; k < kMaxCount; k++) {
					if (auto opt = GetHandleObjectIfLv3(i, j, k); opt.has_value() && opt.value().IsValid()) {
						return iterator(this, i, j, k);
					}
				}
			}
		}
		break;
	}
	std::_Xruntime_error("HandleTable中找不到起始项!");
}

HandleTable::iterator HandleTable::end() const {
	return iterator(this, ~0, ~0, ~0);
}

std::optional<ObjectHeader> HandleTable::GetHandleObjectIfLv1(uint64_t index) const {
	return GetHandleObjectInLv1TableCode(TablePtr(), index);
}
std::optional<ObjectHeader> HandleTable::GetHandleObjectIfLv2(uint64_t index_lv2, uint64_t index_lv1) const {
	return GetHandleObjectInLv2TableCode(TablePtr(), index_lv2, index_lv1);
}
std::optional<ObjectHeader> HandleTable::GetHandleObjectIfLv3(uint64_t index_lv3, uint64_t index_lv2, uint64_t index_lv1) const {
	return GetHandleObjectInLv3TableCode(TablePtr(), index_lv3, index_lv2, index_lv1);
}


std::optional<ObjectHeader> HandleTable::GetHandleObjectInLv1TableCode(uint64_t* table, uint64_t index)
{
	if (MmIsAddressValid(table) && index < 512)
		return ObjectHeader(
			DecryptHandleAddress(table[index]) - ObjectHeader::BodyOffset);
	return std::nullopt;
}

std::optional<ObjectHeader> HandleTable::GetHandleObjectInLv2TableCode(uint64_t* table, uint64_t index_lv2, uint64_t index_lv1)
{
	if (MmIsAddressValid(table) && index_lv2 < 512 && index_lv1 < 512)
		return GetHandleObjectInLv1TableCode((uint64_t*)table[index_lv2], index_lv1);
	return std::nullopt;
}

std::optional<ObjectHeader> HandleTable::GetHandleObjectInLv3TableCode(uint64_t* table, uint64_t index_lv3, uint64_t index_lv2, uint64_t index_lv1)
{
	if (MmIsAddressValid(table) && index_lv3 < 512 && index_lv2 < 512 && index_lv1 < 512)
		return GetHandleObjectInLv2TableCode((uint64_t*)table[index_lv3], index_lv2, index_lv1);
	return std::nullopt;
}


namespace details {
HandleTableIterator::HandleTableIterator(const HandleTable* table, uint16_t idx_lv3, uint16_t idx_lv2, uint16_t idx_lv1)
	: table_(table),
	idx_lv1_(idx_lv1),
	idx_lv2_(idx_lv2),
	idx_lv3_(idx_lv3) {
	CheckValidIndexAndAssign();
}

ObjectHeader& HandleTableIterator::operator*() {
	return cur_obj_;
}
ObjectHeader* HandleTableIterator::operator->() {
	return &cur_obj_;
}
HandleTableIterator& HandleTableIterator::operator++() {
	do {
		if (!JudgeIncIdxOrInEnd()) goto _ret;
	} while (!CheckValidIndexAndAssign());
_ret:
	return *this;
}
HandleTableIterator& HandleTableIterator::operator--() {
	return *this;
}
HandleTableIterator HandleTableIterator::operator++(int) {
	auto tmp = *this;
	++(*this);
	return tmp;
}
HandleTableIterator HandleTableIterator::operator--(int) {
	auto tmp = *this;
	--(*this);
	return tmp;
}
bool HandleTableIterator::operator==(const HandleTableIterator& x) const {
	return x.idx_lv1_ == idx_lv1_ && x.idx_lv2_ == idx_lv2_ && x.idx_lv3_ == idx_lv3_;
}
bool HandleTableIterator::operator!=(const HandleTableIterator& x) const {
	return !operator==(x);
}
bool HandleTableIterator::CheckValidIndexAndAssign()
{
	std::optional<ObjectHeader> tmp;
	switch (table_->TableLevel())
	{
	case 0:
		tmp = table_->GetHandleObjectIfLv1(idx_lv1_);
		break;
	case 1:
		tmp = table_->GetHandleObjectIfLv2(idx_lv2_, idx_lv1_);
		break;
	case 2:
		tmp = table_->GetHandleObjectIfLv3(idx_lv3_, idx_lv2_, idx_lv1_);
		break;
	}
	if (tmp.has_value() && tmp.value().IsValid()) {
		cur_obj_ = tmp.value();
		return true;
	}
	return false;
}
bool HandleTableIterator::JudgeIncIdxOrInEnd()
{
	if (idx_lv1_ == ~0) return false;
	if (idx_lv1_ == HandleTable::kMaxIndex) {
		switch (table_->TableLevel())
		{
		case 0:
		_in_end:
			idx_lv1_ = ~0;
			idx_lv2_ = ~0;
			idx_lv3_ = ~0;
			return false;
		case 1:
			if (idx_lv2_ == HandleTable::kMaxIndex) {
				goto _in_end;
			}
			else {
			_inc_idx2:
				idx_lv1_ = 0;
				if (!FindValidIfLv2Table()) goto _in_end;
			}
			break;
		case 2:
			if (idx_lv2_ == HandleTable::kMaxIndex) {
				if (idx_lv3_ == HandleTable::kMaxIndex) {
					goto _in_end;
				}
				else {
					idx_lv1_ = 0;
					idx_lv2_ = 0;
					if (!FindValidIfLv3Table()) goto _in_end;
				}
			}
			else goto _inc_idx2;
			break;
		}
	}
	else {
		++idx_lv1_;
	}
	return true;
}
bool HandleTableIterator::FindValidIfLv1Table() {
	while(idx_lv1_ < HandleTable::kMaxCount) {
		++idx_lv1_;
		if (MmIsAddressValid((void*)table_->TablePtr()[idx_lv1_])) {
			return true;
		}
	}
	return false;
}
bool HandleTableIterator::FindValidIfLv2Table() {
	while(idx_lv2_ < HandleTable::kMaxCount) {
		++idx_lv2_;
		if (MmIsAddressValid((void*)table_->TablePtr()[idx_lv2_])) {
			return true;
		}
	}
	return false;
}
bool HandleTableIterator::FindValidIfLv3Table() {
	while(idx_lv3_ < HandleTable::kMaxCount) {
		++idx_lv3_;
		if (MmIsAddressValid((void*)table_->TablePtr()[idx_lv3_])) {
			return true;
		}
	}
	return false;
}
}
}