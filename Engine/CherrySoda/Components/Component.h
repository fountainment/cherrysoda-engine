#ifndef _CHERRYSODA_COMPONENTS_COMPONENT_H_
#define _CHERRYSODA_COMPONENTS_COMPONENT_H_

#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/NumType.h>
#include <CherrySoda/Util/String.h>


#define CHERRYSODA_DECLARE_COMPONENT(COMP,BASE) \
typedef BASE base; \
virtual cherrysoda::type::Int32 TypeID() const override \
{ return COMP::ComponentTypeID(); } \
virtual const char* TypeCStr() const override \
{ return #COMP; } \
static CHERRYSODA_STRINGID_CONSTEXPR cherrysoda::type::Int32 ComponentTypeID() \
{ return cherrysoda::StringID(#COMP).GetID(); }


namespace cherrysoda {

class Camera;
class Entity;
class Scene;

class Component
{
public:
	friend class ComponentList;

	Component(bool active, bool visible);
	virtual ~Component() = default;

	CHERRYSODA_GETTER_SETTER_OF_BOOL(Active, m_active);
	CHERRYSODA_GETTER_SETTER_OF_BOOL(Visible, m_visible);

	virtual void Added(Entity* entity);
	virtual void Removed(Entity* entity);

	virtual void EntityAdded(Scene* scene);
	virtual void EntityRemoved(Scene* scene);

	virtual void SceneEnd(Scene* scene);
	virtual void EntityAwake();

	virtual void Update();
	virtual void Render();

	virtual void DebugRender(Camera* camera); 

	virtual void HandleGraphicsReset();
	virtual void HandleGraphicsCreate();

	virtual type::Int32 TypeID() const = 0;
	virtual const char* TypeCStr() const = 0;

	void RemoveSelf();	

	template <class T>
	T* SceneAs() { return static_cast<T*>(GetScene()); }

	template <class T>
	T* EntityAs() { return static_cast<T*>(GetEntity()); }

	inline Entity* GetEntity() { return m_entity; };
	inline const Entity* GetEntity() const { return m_entity; };
	Scene* GetScene() const;

private:
	Entity* m_entity = nullptr;

	bool m_active;
	bool m_visible;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_COMPONENTS_COMPONENT_H_
