#pragma once
#include <jxy/list.hpp>
#include <jxy/vector.hpp>
#include <jxy/string.hpp>
#include <jxy/deque.hpp>
#include <jxy/map.hpp>
#include <jxy/stack.hpp>
#include <jxy/queue.hpp>
#include <jxy/set.hpp>
#include <ntifs.h>

namespace StarryEye {
// 数据结构
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

template <class T, class DeleterT = jxy::default_delete<T, PagedPool, kDefaultPoolTag>>
using unique_ptr = std::unique_ptr<T, DeleterT>;

template <class T>
using shared_ptr = std::shared_ptr<T>;

template <class T, class... ArgsT>
inline unique_ptr<T> make_unique(ArgsT&&... args)
{
	return jxy::make_unique<T, PagedPool, kDefaultPoolTag, ArgsT...>(std::forward<ArgsT>(args)...);
}

template <class T, class... ArgsT>
inline shared_ptr<T> make_shared(ArgsT&&... args)
{
	return jxy::make_shared<T, PagedPool, kDefaultPoolTag, ArgsT...>(std::forward<ArgsT>(args)...);
}

}
}