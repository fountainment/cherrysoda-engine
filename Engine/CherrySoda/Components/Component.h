#ifndef _CHERRYSODA_COMPONENTS_COMPONENT_H_
#define _CHERRYSODA_COMPONENTS_COMPONENT_H_

#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/NumType.h>
#include <CherrySoda/Util/String.h>


#ifndef NDEBUG
#define CHERRYSODA_COMPONENT_TYPE_STR(NAME) \
virtual cherrysoda::String TypeStr() const override \
{ return cherrysoda::StringID(""#NAME).GetStr(); }
#else
#define CHERRYSODA_COMPONENT_TYPE_STR(NAME)
#endif // NDEBUG

#define CHERRYSODA_DECLARE_COMPONENT(COMP,BASE) \
typedef BASE base; \
virtual cherrysoda::type::Int32 TypeID() const override \
{ return COMP::ComponentTypeID(); } \
CHERRYSODA_COMPONENT_TYPE_STR(COMP) \
static cherrysoda::type::Int32 ComponentTypeID() \
{ return cherrysoda::StringID(""#COMP).GetID(); }


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

#ifndef NDEBUG
	virtual String TypeStr() const = 0;
#endif

	void RemoveSelf();	

	template <class T>
	T* SceneAs() { return static_cast<T*>(GetScene()); }

	template <class T>
	T* EntityAs() { return static_cast<T*>(GetEntity()); }

	inline Entity* GetEntity() { return m_entity; };
	inline const Entity* GetEntity() const { return m_entity; };
	Scene* GetScene() const;

	CHERRYSODA_GETTER_SETTER_OF_BOOL(Active, m_active);
	CHERRYSODA_GETTER_SETTER_OF_BOOL(Visible, m_visible);

private:
	Entity* m_entity = nullptr;

	bool m_active;
	bool m_visible;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_COMPONENTS_COMPONENT_H_
