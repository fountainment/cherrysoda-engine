#ifndef _CHERRYSODA_UTIL_STL_H_
#define _CHERRYSODA_UTIL_STL_H_

#include <algorithm>
#include <array>
#include <functional>
#include <list>
#include <map>
#include <queue>
#include <random>
#include <set>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#define CHERRYSODA_ITERABLE(iterable) \
auto begin(){return iterable.begin();} \
auto end(){return iterable.end();} \
auto cbegin()const{return iterable.cbegin();} \
auto cend()const{return iterable.cend();} \
auto rbegin(){return iterable.rbegin();} \
auto rend(){return iterable.rend();} \
auto crbegin()const{return iterable.crbegin();} \
auto crend()const{return iterable.crend();}

#define CHERRYSODA_HASHABLE(hashable,method) \
namespace std{template<>struct hash<hashable> \
{std::size_t operator()(const hashable& v)const \
{auto h=v.method();return std::hash<decltype(h)>()(h);}};}

namespace cherrysoda {

class STL
{
public:
	// Util
	template <typename ...T>
	using Action = std::function<void(T...)>;

	template <typename T, typename ...U>
	using Func = std::function<T(U...)>;

	template <typename T>
	static inline void Swap(T& a, T& b)
	{
		std::swap(a, b);
	}

	// Container
	template <typename T>
	using Queue = std::queue<T>;

	template <typename T>
	using PriorityQueue = std::priority_queue<T, std::vector<T>, std::less<T>>;

	template <typename T>
	using PriorityQueueMinTop = std::priority_queue<T, std::vector<T>, std::greater<T>>;

	template <typename T, typename U>
	using HashMap = std::unordered_map<T,U>;

	template <typename T>
	using HashSet = std::unordered_set<T>;

	template <typename T>
	using List = std::list<T>;

	template <typename T, typename U>
	using Map = std::map<T,U>;

	template <typename T, typename U>
	using Pair = std::pair<T,U>;

	template <typename T, typename U>
	static constexpr Pair<T,U> MakePair(T t, U u)
	{
		return std::make_pair(t, u);
	}

	template <typename T>
	using Set = std::set<T>;

	template <typename T>
	using Stack = std::stack<T>;

	template <typename T>
	using Vector = std::vector<T>;

	template <typename T, size_t S>
	using Array = std::array<T, S>;

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
	static inline void Add(Set<T>& container, const T& element)
	{
		container.emplace(element);
	}

	template <typename T>
	static inline void Add(HashSet<T>& container, const T& element)
	{
		container.emplace(element);
	}

	template <typename T, typename U>
	static inline void Add(Map<T,U>& container, Pair<T,U>&& element)
	{
		container.emplace(element);
	}

	template <typename T, typename U>
	static inline void Add(HashMap<T,U>& container, Pair<T,U>&& element)
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

	template <typename T>
	static inline auto Front(T& container)
	{
		return container.front();
	}

	template <typename T>
	static inline auto Back(T& container)
	{
		return container.back();
	}

	template <typename T>
	static inline auto Pop(Stack<T>& container)
	{
		auto element = container.top();
		container.pop();
		return element;
	}

	template <typename T>
	static inline auto Pop(Queue<T>& container)
	{
		auto element = container.front();
		container.pop();
		return element;
	}

	template <typename T>
	static inline auto Pop(PriorityQueue<T>& container)
	{
		auto element = container.top();
		container.pop();
		return element;
	}

	template <typename T>
	static inline auto Pop(PriorityQueueMinTop<T>& container)
	{
		auto element = container.top();
		container.pop();
		return element;
	}

	template <typename T, typename U>
	static inline void Push(T& container, const U& element)
	{
		container.push(element);
	}

	template <typename T>
	static inline void Sort(Vector<T>& container)
	{
		std::sort(container.begin(), container.end());
	}

	template <typename T, typename Compare>
	static inline void Sort(Vector<T>& container, Compare comp)
	{
		std::sort(container.begin(), container.end(), comp);
	}

	template <typename T>
	static inline void Sort(List<T>& container)
	{
		container.sort();
	}

	template <typename T, typename Compare>
	static inline void Sort(List<T>& container, Compare comp)
	{
		container.sort(comp);
	}

	template <typename T>
	static inline auto Top(T& container)
	{
		return container.top();
	}

	template <typename T>
	static inline auto& TopRef(T& container)
	{
		return container.top();
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
	static inline bool IsEmpty(const T& container)
	{
		return container.empty();
	}

	template <typename T>
	static inline bool IsNotEmpty(const T& container)
	{
		return !container.empty();
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
	static inline bool RemoveKey(T& container, const typename T::key_type& key)
	{
		auto it = FindKey(container, key);
		if (it != container.end()) {
			container.erase(it);
			return true;
		}
		return false;
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
	static inline const typename T::mapped_type* GetValueAddress(const T& container, const typename T::key_type& key)
	{
		auto it = FindKey(container, key);
		if (it != container.end()) {
			return &it->second;
		}
		return nullptr;
	}

	template <typename T>
	static inline bool TryGetFirst(const T& container, typename T::value_type& output)
	{
		auto it = container.begin();
		if (it != container.end()) {
			output = *it;
			return true;
		}
		return false;
	}

	template <typename T>
	static inline Vector<typename T::value_type> ToVector(T& container)
	{
		return Vector<typename T::value_type>(std::begin(container), std::end(container));
	}

	// Random
	using Random = std::mt19937;
	using TrueRandom = std::random_device;
	template <typename T>
	static inline auto RandomNext(T& random) { return random() - random.min(); }
	template <typename T>
	static inline auto RandomMax(T& random) { return random.max() - random.min(); }
	template <typename T>
	static inline auto RandomMin(T& random) { return 0; }
	template <typename T>
	static inline void RandomSeed(T& random, Random::result_type seed) { return random.seed(seed); }

	template <typename T>
	static inline void Shuffle(T& container)
	{
		std::random_shuffle(container.begin(), container.end());
	}

	template <typename T, typename U>
	static inline void Shuffle(T& container, U& random)
	{
		std::random_shuffle(container.begin(), container.end(), random);
	}
};

} // namespace cherrysoda

#endif // _CHERRYSODA_UTIL_STL_H_
