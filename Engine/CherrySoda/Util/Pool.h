#ifndef _CHERRYSODA_UTIL_POOL_H_
#define _CHERRYSODA_UTIL_POOL_H_

#include <CherrySoda/Util/Log.h>
#include <CherrySoda/Util/NumType.h>
#include <CherrySoda/Util/STL.h>

namespace cherrysoda {

class PoolInterface
{
public:
	virtual void INTERNAL_Hide(void* ptr) = 0;
	virtual void INTERNAL_Destroy(void* ptr) = 0;
};

template <typename T, type::UInt16 SIZE>
class alignas(128) Pool : public PoolInterface
{
public:
	typedef type::UInt16 SizeType;

	Pool()
	{
		for (SizeType i = SIZE; i != 0; --i) {
			STL::Push(m_available, i - 1);
		}
	}

	~Pool()
	{
		INTERNAL_Clear();
	}

	inline bool IsFull() { return STL::IsEmpty(m_available); }

	template <typename... TT>
	T* Create(TT... TTs)
	{
		CHERRYSODA_ASSERT(!IsFull(), "Pool is full!\n");
		if (IsFull()) return nullptr;
		SizeType loc = STL::Pop(m_available);
		STL::Add(m_allocated, loc);
		auto t = new((T*)m_buffer + loc) T(TTs...);
		t->AutoDeleteWhenRemoved(this);
		return t;
	}

	void INTERNAL_Hide(void* ptr) override
	{
		SizeType loc = (T*)ptr - (T*)m_buffer;
		CHERRYSODA_ASSERT(STL::Contains(m_allocated, loc), "Element not exist in this pool!\n");
		STL::Remove(m_allocated, loc);
	}

	void INTERNAL_Destroy(void* ptr) override
	{
		SizeType loc = (T*)ptr - (T*)m_buffer;
		((T*)ptr)->~T();
		STL::Push(m_available, loc);
	}

	void Traverse(STL::Action<T*> action)
	{
		for (SizeType loc : m_allocated) {
			action((T*)m_buffer + loc);
		}
	}

	void INTERNAL_Clear()
	{
		for (SizeType loc : m_allocated) {
			((T*)m_buffer + loc)->~T();
			STL::Push(m_available, loc);
		}
		STL::Clear(m_allocated);
	}

private:
	alignas(64) STL::Stack<SizeType> m_available;
	alignas(64) STL::Set<SizeType> m_allocated;
	alignas(64) char m_buffer[SIZE * sizeof(T)] = { 0 };
};

} // namespace cherrysoda

#define CHERRYSODA_FRIEND_CLASS_POOL template <typename T, type::UInt16 SIZE> friend class Pool

#endif // _CHERRYSODA_UTIL_POOL_H_
