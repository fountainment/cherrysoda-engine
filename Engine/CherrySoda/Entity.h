#ifndef _CHERRYSODA_ENTITY_H_
#define _CHERRYSODA_ENTITY_H_

#include <CherrySoda/InternalUtilities/ComponentList.h>

#include <CherrySoda/Util/BitTag.h>
#include <CherrySoda/Util/Math.h>

namespace cherrysoda {

class Camera;
class Component;
class ComponentList;
class Scene;

class Entity
{
public:
	Entity() : Entity(Math::Vec3(0.f)) {}
	Entity(const Math::Vec3& position);
	
	virtual void SceneBegin(Scene* scene);
	virtual void SceneEnd(Scene* scene);
	virtual void Awake(Scene* scene);

	virtual void Update();
	virtual void Render();

	virtual void DebugRender(Camera* camera);

	virtual void HandleGraphicsReset();
	virtual void HandleGraphicsCreate();

	void RemoveSelf();

	inline bool TagCheck(BitTagValueType tag) const { return (m_tag & tag) != 0; }
	inline bool TagFullCheck(BitTagValueType tag) const { return (m_tag & tag) == tag; }
	inline void AddTag(BitTagValueType tag) { m_tag |= tag; }
	inline void RemoveTag(BitTagValueType tag) { m_tag &= ~tag; }

	void Add(Component* component); 
	void Remove(Component* component);
	void Add(ComponentList::IterableComponents& components); 
	void Remove(ComponentList::IterableComponents& components);

	void Depth(int depth);
	inline int Depth() const { return m_depth; }

	template<class T>
	T* Get() { return m_components != nullptr ? m_components->Get<T>() : nullptr; }

	inline Scene* GetScene() { return m_scene; }

	static bool CompareDepth(Entity* a, Entity* b) { return a->m_actualDepth < b->m_actualDepth; }

	CHERRYSODA_GETTER_SETTER_OF_VEC3(Position, m_position);
	CHERRYSODA_GETTER_SETTER_OF_TYPE(bool, Active, m_active);
	CHERRYSODA_GETTER_SETTER_OF_TYPE(bool, Visible, m_visible);

private:
	friend class EntityList;
	friend class Scene;

	virtual void Added(Scene* scene);
	virtual void Removed(Scene* scene);

	Math::Vec3 m_position = Math::Vec3(0.f);
	bool m_active = true;
	bool m_visible = true;
	bool m_collidable = false;

	Scene* m_scene = nullptr;
	ComponentList* m_components = nullptr;

	int m_id = 0;
	BitTagValueType m_tag = 0;

	// Collider* collider;

	int m_depth = 0;
	double m_actualDepth = 0.0;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_ENTITY_H_
