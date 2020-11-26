#ifndef _CHERRYSODA_INTERNALUTILITIES_TAGLISTS_H_
#define _CHERRYSODA_INTERNALUTILITIES_TAGLISTS_H_

#include <CherrySoda/Util/STL.h>

namespace cherrysoda {

class Entity;

class TagLists
{
public:
	const STL::List<Entity*>& operator [] (int index) const;

private:
	friend class Entity;
	friend class EntityList;
	friend class Scene;

	TagLists();

	STL::List<Entity*>& operator [] (int index);

	void MarkUnsorted(int index);
	void UpdateLists();
	void EntityAdded(Entity* entity);
	void EntityRemoved(Entity* entity);

	STL::Vector<STL::List<Entity*>> m_lists;
	STL::Vector<bool> m_unsorted;
	bool m_areAnyUnsorted;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_INTERNALUTILITIES_TAGLISTS_H_
