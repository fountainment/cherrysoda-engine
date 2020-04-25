#ifndef _CHERRYSODA_UTIL_STL_H_
#define _CHERRYSODA_UTIL_STL_H_

#include <algorithm>
#include <functional>
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#define CHERRYSODA_ITERABLE(iterable) \
auto begin(){return iterable.begin();} \
auto end(){return iterable.end();} \
auto cbegin()const{return iterable.cbegin();} \
auto cend()const{return iterable.cend();}

#define CHERRYSODA_HASHABLE(hashable,method) \
namespace std{template<>struct hash<hashable> \
{std::size_t operator()(const hashable& v)const \
{auto h=v.method();return std::hash<decltype(h)>()(h);}};}

namespace cherrysoda {

class STL
{
public:
	template <typename ...T>
	using Action = std::function<void(T...)>;

	template <typename T, typename U>
	using HashMap = std::unordered_map<T,U>;

	template <typename T>
	using HashSet = std::unordered_set<T>;

	template <typename T>
	using List = std::list<T>;

	template <typename T, typename U>
	using Map = std::map<T,U>;

	template <typename T>
	using Set = std::set<T>;

	template <typename T>
	using Vector = std::vector<T>;

	template <typename T>
	static inline void Resize(T& container, size_t size)
	{
		container.resize(size);
	}

	template <typename T>
	static inline void Reserve(T& container, size_t size)
	{
		container.reserve(size);
	}

	template <typename T, typename U>
	static inline void Fill(T& container, const U& element)
	{
		std::fill(container.begin(), container.end(), element);
	}

	template <typename T, typename U>
	static inline void Add(T& container, const U& element)
	{
		container.emplace_back(element);
	}

	template <typename T>
	static inline void Add(HashSet<T>& container, const T& element)
	{
		container.emplace(element);
	}

	template <typename T>
	static inline void AddRange(T& container, const T& iterable)
	{
		container.insert(std::end(container), std::begin(iterable), std::end(iterable));
	}

	template <typename T, typename U>
	static inline void AddRange(T& container, const U& iterable)
	{
		container.insert(std::end(container), std::begin(iterable), std::end(iterable));
	}

	template <typename T, typename U>
	static inline bool Remove(T& container, const U& element)
	{
		auto it = Find(container, element);
		if (it == container.end()) {
			return false;
		}
		container.erase(it);
		return true;
	}

	template <typename T>
	static inline size_t Count(const T& container)
	{
		return container.size();
	}

	template <typename T>
	static inline auto Data(const T& container)
	{
		return container.data();
	}

	template <typename T>
	static inline auto Data(T& container)
	{
		return container.data();
	}

	template <typename T>
	static inline void Clear(T& container)
	{
		container.clear();
	}

	template <typename T>
	static inline size_t ByteSize(const T& container)
	{
		return Count(container) * sizeof (typename T::value_type);
	}

	template <typename T>
	static inline auto Find(const T& container, const typename T::value_type& element)
	{
		return std::find(std::begin(container), std::end(container), element);
	}

	template <typename T>
	static inline auto Find(const Set<T>& container, const T& element)
	{
		return container.find(element);
	}

	template <typename T>
	static inline auto Find(const HashSet<T>& container, const T& element)
	{
		return container.find(element);
	}

	template <typename T>
	static inline auto FindKey(const T& container, const typename T::key_type& key)
	{
		return container.find(key);
	}

	template <typename T>
	static inline bool Contains(const T& container, const typename T::value_type& element)
	{
		return Find(container, element) != container.end();
	}

	template <typename T>
	static inline bool ContainsKey(const T& container, const typename T::key_type& key)
	{
		return FindKey(container, key) != container.end();
	}

	template <typename T>
	static inline bool TryGetValue(const T& container, const typename T::key_type& key, typename T::mapped_type& output)
	{
		auto it = FindKey(container, key);
		if (it != container.end()) {
			output = it->second;
			return true;
		}
		return false;
	}

	template <typename T>
	static inline void Shuffle(T& container)
	{
		std::random_shuffle(container.begin(), container.end());
	}

	template <typename T>
	static inline void Swap(T& a, T& b)
	{
		std::swap(a, b);
	}

	template <typename T>
	static inline Vector<typename T::value_type> ToVector(T& container)
	{
		return Vector<typename T::value_type>(std::begin(container), std::end(container));
	}
};

} // namespace cherrysoda

#endif // _CHERRYSODA_UTIL_STL_H_
