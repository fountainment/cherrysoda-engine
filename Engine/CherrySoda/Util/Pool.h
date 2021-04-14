#ifndef _CHERRYSODA_UTIL_POOL_H_
#define _CHERRYSODA_UTIL_POOL_H_

#include <CherrySoda/Util/NumType.h>
#include <CherrySoda/Util/STL.h>

namespace cherrysoda {

template <typename ElementType, type::UInt16 Size>
class Pool
{
public:
	typedef type::UInt16 SizeT;

	Pool()
	{
		for (SizeT i = Size; i != 0; --i) {
			STL::Push(m_available, i - 1);
		}
	}

	~Pool()
	{
		bool available[Size] = { 0 };
		while (!m_available.empty()) {
			available[STL::Pop(m_available)] = true;
		}
		for (int loc = 0; loc < Size; ++loc) {
			if (!available[loc]) {
				((ElementType*)m_buffer + loc)->~ElementType();
			}
		}
	}

	template <typename... T>
	ElementType* Create(T... Ts)
	{
		CHERRYSODA_ASSERT(STL::IsNotEmpty(m_available), "Pool is full!\n");
		SizeT loc = STL::Pop(m_available);
		return new((ElementType*)m_buffer + loc) ElementType(Ts...);
	}

	void Destroy(void* ptr)
	{
		((ElementType*)ptr)->~ElementType();
		STL::Push(m_available, (ElementType*)ptr - (ElementType*)m_buffer);
	}

private:
	STL::Queue<SizeT> m_available;
	char m_buffer[Size * sizeof(ElementType)] = { 0 };
};

} // namespace cherrysoda

#endif // _CHERRYSODA_UTIL_POOL_H_
