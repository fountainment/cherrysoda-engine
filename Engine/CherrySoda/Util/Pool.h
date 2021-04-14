#ifndef _CHERRYSODA_UTIL_POOL_H_
#define _CHERRYSODA_UTIL_POOL_H_

#include <CherrySoda/Util/NumType.h>
#include <CherrySoda/Util/STL.h>

namespace cherrysoda {

template <typename T, type::UInt16 SIZE>
class alignas(128) Pool
{
public:
	typedef type::UInt16 SizeType;
	typedef STL::Stack<SizeType> ContainerType;

	Pool()
	{
		for (SizeType i = SIZE; i != 0; --i) {
			STL::Push(m_available, i - 1);
		}
	}

	~Pool()
	{
		bool available[SIZE] = { 0 };
		while (!m_available.empty()) {
			available[STL::Pop(m_available)] = true;
		}
		for (int loc = 0; loc < SIZE; ++loc) {
			if (!available[loc]) {
				((T*)m_buffer + loc)->~T();
			}
		}
	}

	template <typename... TT>
	T* Create(TT... TTs)
	{
		CHERRYSODA_ASSERT(STL::IsNotEmpty(m_available), "Pool is full!\n");
		SizeType loc = STL::Pop(m_available);
		return new((T*)m_buffer + loc) T(TTs...);
	}

	void Destroy(void* ptr)
	{
		((T*)ptr)->~T();
		STL::Push(m_available, (T*)ptr - (T*)m_buffer);
	}

private:
	alignas(64) ContainerType m_available;
	alignas(64) char m_buffer[SIZE * sizeof(T)] = { 0 };
};

} // namespace cherrysoda

#endif // _CHERRYSODA_UTIL_POOL_H_
