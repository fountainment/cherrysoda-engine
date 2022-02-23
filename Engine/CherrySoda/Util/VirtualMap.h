#ifndef _CHERRYSODA_UTIL_VIRTUALMAP_H_
#define _CHERRYSODA_UTIL_VIRTUALMAP_H_

#include <CherrySoda/Util/STL.h>

namespace cherrysoda {

template <class T>
class VirtualMap
{
private:
	static constexpr int ms_segmentSize = 50;

public:
	typedef STL::Array<STL::Array<T, ms_segmentSize>, ms_segmentSize> SegmentType;

	VirtualMap(int columns, int rows, const T& emptyValue = T{})
	{
		m_columns = columns;
		m_rows = rows;
		m_segmentColumns = (columns / ms_segmentSize) + 1;
		m_segmentRows = (rows / ms_segmentSize) + 1;
		m_segments = STL::Vector<STL::Vector<SegmentType*>>(m_segmentColumns, STL::Vector<SegmentType*>(m_segmentRows, nullptr));
		m_emptyValue = emptyValue;
	}

	~VirtualMap()
	{
		for (auto& rows : m_segments) {
			for (auto& segment : rows) {
				if (segment != nullptr) {
					delete segment;
				}
			}
		}
		STL::Clear(m_segments);
	}

	T Get(int x, int y) const
	{
		int cx = x / ms_segmentSize;
		int cy = y / ms_segmentSize;

		auto seg = m_segments[cx][cy];
		if (seg == nullptr) {
			return m_emptyValue;	
		}
		return (*seg)[x - cx * ms_segmentSize][y - cy * ms_segmentSize];
	}

	void Set(int x, int y, const T& value)
	{
		int cx = x / ms_segmentSize;
		int cy = y / ms_segmentSize;

		if (m_segments[cx][cy] == nullptr) {
			if (value == m_emptyValue) return;
			m_segments[cx][cy] = new SegmentType();
			if (m_emptyValue != T{}) {
				for (int tx = 0; tx < ms_segmentSize; ++tx)
					for (int ty = 0; ty < ms_segmentSize; ++ty)
						(*m_segments[cx][cy])[tx][ty] = m_emptyValue;	
			}
		}
		(*m_segments[cx][cy])[x - cx * ms_segmentSize][y - cy * ms_segmentSize] = value;
	}

	inline int Columns() const { return m_columns; }
	inline int Rows() const { return m_rows; }

private:
	int m_columns;
	int m_rows;
	int m_segmentColumns;
	int m_segmentRows;	
	T m_emptyValue;

	STL::Vector<STL::Vector<SegmentType*>> m_segments;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_UTIL_VIRTUALMAP_H_
