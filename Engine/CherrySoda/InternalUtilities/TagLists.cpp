#include <CherrySoda/InternalUtilities/TagLists.h>

#include <CherrySoda/Entity.h>
#include <CherrySoda/InternalUtilities/EntityList.h>
#include <CherrySoda/Util/BitTag.h>
#include <CherrySoda/Util/STL.h>

using cherrysoda::TagLists;

using cherrysoda::BitTag;
using cherrysoda::Entity;
using cherrysoda::EntityList;
using cherrysoda::STL;

TagLists::TagLists()
{
	m_lists = STL::Vector<STL::List<Entity*>>(BitTag::TotalTags());
	m_unsorted = STL::Vector<bool>(BitTag::TotalTags());
}

const STL::List<Entity*>& TagLists::operator [] (int index) const
{
	return m_lists[index];
}

STL::List<Entity*>& TagLists::operator [] (int index)
{
	return m_lists[index];
}

void TagLists::MarkUnsorted(int index)
{
	m_areAnyUnsorted = true;
	m_unsorted[index] = true;
}

void TagLists::UpdateLists()
{
	if (!m_areAnyUnsorted) {
		return;	
	}
	for (int i = 0; i < BitTag::TotalTags(); ++i) {
		if (m_unsorted[i]) {
			STL::Sort(m_lists[i], EntityList::CompareDepth);
			m_unsorted[i] = false;
		}
	}
	m_areAnyUnsorted = false;
}

void TagLists::EntityAdded(Entity* entity)
{
	for (int i = 0; i < BitTag::TotalTags(); ++i)
	{
		if (entity->TagCheck(1 << i))
		{
			STL::Add(m_lists[i], entity);
			m_areAnyUnsorted = true;
			m_unsorted[i] = true;
		}
	}
}

void TagLists::EntityRemoved(Entity* entity)
{
	for (int i = 0; i < BitTag::TotalTags(); ++i)
	{
		if (entity->TagCheck(1 << i))
		{
			STL::Remove(m_lists[i], entity);
		}
	}
}
