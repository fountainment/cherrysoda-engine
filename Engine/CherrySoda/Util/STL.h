#ifndef _CHERRYSODA_UTIL_STL_H_
#define _CHERRYSODA_UTIL_STL_H_

#include <algorithm>
#include <functional>
#include <list>
#include <unordered_set>
#include <vector>

#define CHERRYSODA_ITERABLE(iterable) \
	decltype(iterable)::iterator begin() { return iterable.begin(); } \
	decltype(iterable)::iterator end()   { return iterable.end(); }

namespace cherrysoda {

class STL
{
public:
	using Action = std::function<void()>;

	template <typename T>
	using HashSet = std::unordered_set<T>;

	template <typename T>
	using List = std::list<T>;

	template <typename T>
	using Vector = std::vector<T>;

	template<class T>
	static void Resize(T& container, size_t size)
	{
		container.resize(size);
	}

	template<class T, class U>
	static void Fill(T& container, const U& element)
	{
		std::fill(container.begin(), container.end(), element);
	}

	template<class T, class U>
	static void Add(T& container, const U& element)
	{
		container.emplace_back(element);
	}

	template<class T>
	static void Add(HashSet<T>& container, const T& element)
	{
		container.emplace(element);
	}

	template<class T, class U>
	static void AddRange(T& container, const U& iterable)
	{
		container.insert(std::end(container), std::begin(iterable), std::end(iterable));
	}

	template<class T, class U>
	static bool Remove(T& container, const U& element)
	{
		auto it = Find(container, element);
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

	template<class T>
	static auto Data(const T& container)
	{
		return container.data();
	}

	template<class T>
	static size_t ByteSize(const T& container)
	{
		return Count(container) * sizeof T::value_type;
	}

	template<class T, class U>
	static auto Find(const T& container, const U& element)
	{
		return std::find(std::begin(container), std::end(container), element);
	}

	template<class T>
	static auto Find(std::unordered_set<T>& container, const T& element)
	{
		return container.find(element);
	}

	template<class T, class U>
	static bool Contains(const T& container, const U& element)
	{
		return Find(container, element) != container.end();
	}
};

} // namespace cherrysoda

#endif // _CHERRYSODA_UTIL_STL_H_