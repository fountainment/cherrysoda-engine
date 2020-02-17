#ifndef _CHERRYSODA_UTIL_STL_H_
#define _CHERRYSODA_UTIL_STL_H_

#include <algorithm>
#include <functional>
#include <list>
#include <map>
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

	template <typename T, typename U>
	using Map = std::map<T,U>;

	template <typename T>
	using Vector = std::vector<T>;

	template<typename T>
	static void Resize(T& container, size_t size)
	{
		container.resize(size);
	}

	template<typename T, typename U>
	static void Fill(T& container, const U& element)
	{
		std::fill(container.begin(), container.end(), element);
	}

	template<typename T, typename U>
	static void Add(T& container, const U& element)
	{
		container.emplace_back(element);
	}

	template<typename T>
	static void Add(HashSet<T>& container, const T& element)
	{
		container.emplace(element);
	}

	template<typename T>
	static void AddRange(T& container, const T& iterable)
	{
		container.insert(std::end(container), std::begin(iterable), std::end(iterable));
	}

	template<typename T, typename U>
	static void AddRange(T& container, const U& iterable)
	{
		container.insert(std::end(container), std::begin(iterable), std::end(iterable));
	}

	template<typename T, typename U>
	static bool Remove(T& container, const U& element)
	{
		auto it = Find(container, element);
		if (it == container.end()) {
			return false;
		}
		container.erase(it);
		return true;
	}

	template<typename T>
	static size_t Count(const T& container)
	{
		return container.size();
	}

	template<typename T>
	static auto Data(const T& container)
	{
		return container.data();
	}

	template<typename T>
	static void Clear(T& container)
	{
		container.clear();
	}

	template<typename T>
	static size_t ByteSize(const T& container)
	{
		return Count(container) * sizeof (typename T::value_type);
	}

	template<typename T, typename U>
	static auto Find(const T& container, const U& element)
	{
		return std::find(std::begin(container), std::end(container), element);
	}

	template<typename T>
	static auto Find(const HashSet<T>& container, const T& element)
	{
		return container.find(element);
	}

	template<typename T, typename U>
	static auto Find(const Map<T,U>& container, const T& key)
	{
		return container.find(key);
	}

	template<typename T, typename U>
	static bool Contains(const T& container, const U& element)
	{
		return Find(container, element) != container.end();
	}

	template<typename T>
	static void Shuffle(T& container)
	{
		std::random_shuffle(container.begin(), container.end());
	}
};

} // namespace cherrysoda

#endif // _CHERRYSODA_UTIL_STL_H_