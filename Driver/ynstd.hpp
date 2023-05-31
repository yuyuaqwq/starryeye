#pragma once
#include <jxy/list.hpp>
#include <jxy/vector.hpp>
#include <jxy/string.hpp>
#include <jxy/deque.hpp>
#include <jxy/map.hpp>
#include <jxy/stack.hpp>
#include <jxy/queue.hpp>
#include <jxy/set.hpp>
#include <ntddk.h>

namespace StarryEye {
namespace ynstd {

static const ULONG kDefaultPoolTag = 114514;

template<class T>
using default_allocator = jxy::paged_allocator<T, kDefaultPoolTag>;

template<class T>
using list = jxy::list<T, PagedPool, kDefaultPoolTag>;
template<class T>
using vector = jxy::vector<T, PagedPool, kDefaultPoolTag>;
template<class T>
using deque = jxy::deque<T, PagedPool, kDefaultPoolTag>;
template<class T>
using stack = jxy::stack<T, PagedPool, kDefaultPoolTag>;
template<class T>
using queue = jxy::queue<T, PagedPool, kDefaultPoolTag>;
template<class T>
using set = jxy::set<T, PagedPool, kDefaultPoolTag>;
template<class T>
using multiset = jxy::multiset<T, PagedPool, kDefaultPoolTag>;

template<class KeyT, class ValueT>
using map = jxy::map<KeyT, ValueT, PagedPool, kDefaultPoolTag>;
template<class KeyT, class ValueT>
using multimap = jxy::multimap<KeyT, ValueT, PagedPool, kDefaultPoolTag>;

using string = jxy::string<PagedPool, kDefaultPoolTag>;
}
}