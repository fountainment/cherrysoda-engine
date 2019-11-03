#ifndef _CHERRYSODA_UTIL_STL_H_
#define _CHERRYSODA_UTIL_STL_H_

#include <algorithm>
#include <unordered_set>

#define CHERRYSODA_ITERABLE(iterable) \
	decltype(iterable)::iterator begin() { return iterable.begin(); } \
	decltype(iterable)::iterator end()   { return iterable.end(); }

namespace cherrysoda {

class STL
{
public:
	template<class T, class U>
	static void Add(T& container, const U& element)
	{
		container.emplace_back(element);
	}

	template<class T>
	static void Add(std::unordered_set<T>& container, const T& element)
	{
		container.emplace(element);
	}

	template<class T, class U>
	static void AddIterable(T& container, const U& iterable)
	{
		container.insert(std::end(container), std::begin(iterable), std::end(iterable));
	}

	template<class T, class U>
	static bool Remove(T& container, const U& element)
	{
		T::iterator it = std::find(container.begin(), container.end(), element);
		if (it == container.end()) {
			return false;
		}
		container.erase(it);
		return true;
	}

	template<class T>
	static size_t Count(const T& container)
	{
		return container.size();
	}

	template<class T, class U>
	static bool Contains(const T& container, const U& element)
	{
		return std::find(container.begin(), container.end(), element) != container.end();
	}
};

} // namespace cherrysoda

#endif // _CHERRYSODA_UTIL_STL_H_