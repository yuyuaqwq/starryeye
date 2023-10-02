#include "handle_table.h"

namespace stareye {
uint64_t HandleTable::DecryptHandleAddress(uint64_t addr)
{
	return ((LONG64)addr >> 0x10) & ~0xF;
}

bool HandleTable::ForeachAllHandleObjectsInLv1TableCode(uint64_t* table, const ForeachHandleObjectsCallBack& callback)
{
	ObjectHeader temp{};
	for (SHORT i = 0; i < 512; i++) {
		if (auto opt = GetHandleObjectInLv1TableCode(table, i); opt.has_value()) {
			if (temp = opt.value(); temp.VAddr().IsValid() && !callback(temp)) {
				return false;
			}
		}
	}
	return true;
}

bool HandleTable::ForeachAllHandleObjectsInLv2TableCode(uint64_t* table, const ForeachHandleObjectsCallBack& callback)
{
	for (size_t i = 0; i < 512; i++) {
		if (MmIsAddressValid(table + i)) {
			if (!HandleTable::ForeachAllHandleObjectsInLv1TableCode((uint64_t*)table[i], callback)) return false;
		}
	}
	return true;
}

bool HandleTable::ForeachAllHandleObjectsInLv3TableCode(uint64_t* table, const ForeachHandleObjectsCallBack& callback)
{
	for (size_t i = 0; i < 512; i++) {
		if (MmIsAddressValid(table + i)) {
			if (!HandleTable::ForeachAllHandleObjectsInLv2TableCode((uint64_t*)table[i], callback)) return false;
		}
	}
	return true;
}

void HandleTable::Init()
{
	TableCodeOffset = 0x8;
	PspCidTable = 0xfffff802598fc5d0;	//TODO PspCidTable
}

HandleTable::HandleTable(const MmVirtualAddress& vaddr) :KObject(vaddr) {}

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

std::optional<ObjectHeader> HandleTable::GetHandleObject(uint64_t index) const
{
	if (index >= MaxTableSize()) return std::nullopt;

	switch (TableLevel())
	{
	case 0:
		return GetHandleObjectInLv1TableCode((uint64_t*)TableAddress(), index);
	case 1:
		return GetHandleObjectInLv2TableCode((uint64_t*)TableAddress(), index / 512, index % 512);
	case 2:
		return GetHandleObjectInLv3TableCode((uint64_t*)TableAddress(), index / (512ULL * 512), (index % (512ULL * 512)) / 512, (index % (512ULL * 512)) % 512);
	default:
		return std::nullopt;
	}
}

HandleTable::iterator HandleTable::begin() {
	uint64_t idx = 0;
	while (true) {
		if (auto opt = GetHandleObject(idx); opt.has_value() && opt.value().VAddr().IsValid()){
			break;
		}
		++idx;
	}
	return HandleTable::iterator(this, idx);
}

HandleTable::iterator HandleTable::end() {
	size_t lv1i = 0;
	size_t lv2i = 0;
	size_t lv3i = 0;
	auto table = (uint64_t*)TableAddress();
	switch (TableLevel())
	{
	case 0:
	_foreach0:
		for (lv1i = 0; lv1i < 512; lv1i++) {
			if (auto opt = GetHandleObjectInLv1TableCode(table, lv1i); !opt.has_value()) {
				break;
			}
		}
		break;
	case 1:
	_foreach1:
		for (lv2i = 0; lv2i < 512; lv2i++) {
			if (!MmIsAddressValid(table + lv2i)) {
				table = (uint64_t*)table[--lv2i];
				goto _foreach0;
			}
		}
		break;
	case 2:
		for (lv3i = 0; lv3i < 512; lv3i++) {
			if (!MmIsAddressValid(table + lv3i)) {
				table = (uint64_t*)table[--lv3i];
				goto _foreach1;
			}
		}
		break;
	}
	return HandleTable::iterator(this, lv3i * 512 * 512 + lv2i * 512 + lv1i);
}

bool HandleTable::AutoForeachAllHandleObjects(const ForeachHandleObjectsCallBack& callback) const
{
	switch (TableLevel())
	{
	case 0:
		HandleTable::ForeachAllHandleObjectsInLv1TableCode((uint64_t*)TableAddress(), callback);
		return true;
	case 1:
		HandleTable::ForeachAllHandleObjectsInLv2TableCode((uint64_t*)TableAddress(), callback);
		return true;
	case 2:
		HandleTable::ForeachAllHandleObjectsInLv3TableCode((uint64_t*)TableAddress(), callback);
		return true;
	default:
		return false;
	}
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
HandleTableIterator::HandleTableIterator(HandleTable* table, uint64_t idx) 
	: table_(table),
	cur_idx_(idx),
	cur_obj_(table_->GetHandleObject(idx).value_or(MmVirtualAddress())) {}

ObjectHeader& HandleTableIterator::operator*() {
	return cur_obj_;
}
ObjectHeader* HandleTableIterator::operator->() {
	return &cur_obj_;
}
HandleTableIterator& HandleTableIterator::operator++() {
	do {
		++cur_idx_;
		if (auto opt = table_->GetHandleObject(cur_idx_); opt.has_value()) {
			cur_obj_ = opt.value();
		}
	} while (!cur_obj_.VAddr().IsValid());
	return *this;
}
HandleTableIterator& HandleTableIterator::operator--() {
	do {
		--cur_idx_;
		if (auto opt = table_->GetHandleObject(cur_idx_); opt.has_value()) {
			cur_obj_ = opt.value();
		}
	} while (!cur_obj_.VAddr().IsValid());
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
bool HandleTableIterator::operator==(const HandleTableIterator& x) {
	return x.cur_idx_ == cur_idx_;
}
bool HandleTableIterator::operator!=(const HandleTableIterator& x) {
	return x.cur_idx_ != cur_idx_;
}
}
}