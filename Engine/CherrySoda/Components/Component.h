#ifndef _CHERRYSODA_COMPONENTS_COMPONENT_H_
#define _CHERRYSODA_COMPONENTS_COMPONENT_H_

namespace cherrysoda {

class Entity;

class Component
{
public:
	friend class ComponentList;

	Component(bool active, bool visible);

	virtual void Added(Entity* entity);
	virtual void Removed(Entity* entity);

	virtual void Update();
	virtual void Render();

	inline Entity* GetEntity() { return m_entity; };

private:
	Entity* m_entity;

	bool m_active;
	bool m_visible;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_COMPONENTS_COMPONENT_H_