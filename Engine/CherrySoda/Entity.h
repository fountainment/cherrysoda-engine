#ifndef _CHERRYSODA_ENTITY_H_
#define _CHERRYSODA_ENTITY_H_

#include <CherrySoda/InternalUtilities/ComponentList.h>

#include <CherrySoda/Util/BitTag.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/STL.h>

namespace cherrysoda {

class Camera;
class Collider;
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

	inline bool TagCheck(BitTagValueType tag) const { return (Tag() & tag) != 0; }
	inline bool TagFullCheck(BitTagValueType tag) const { return (Tag() & tag) == tag; }
	inline void AddTag(BitTagValueType tag) { Tag(Tag() | tag); }
	inline void RemoveTag(BitTagValueType tag) { Tag(Tag() & ~tag); }

	inline int Tag() const { return m_tag; }
	void Tag(int tag);

	const STL::List<Entity*> CollideAll(const BitTag& tag) const;
	bool CollideCheck(const BitTag& tag) const;
	Entity* CollideFirst(const BitTag& tag) const;

	void Add(Component* component); 
	void Remove(Component* component);
	void Add(ComponentList::IterableComponents& components); 
	void Remove(ComponentList::IterableComponents& components);

	void Depth(int depth);
	inline int Depth() const { return m_depth; }

	template<class T>
	T* Get() { return m_components != nullptr ? m_components->Get<T>() : nullptr; }

	inline Scene* GetScene() { return m_scene; }

	inline Collider* GetCollider() const { return m_collider; }
	inline void SetCollider(Collider* collider);

	CHERRYSODA_GETTER_SETTER_OF_VEC3(Position, m_position);
	CHERRYSODA_GETTER_SETTER_OF_BOOL(Active, m_active);
	CHERRYSODA_GETTER_SETTER_OF_BOOL(Visible, m_visible);
	CHERRYSODA_GETTER_SETTER_OF_BOOL(Collidable, m_collidable);

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

	Collider* m_collider = nullptr;

	int m_depth = 0;
	double m_actualDepth = 0.0;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_ENTITY_H_
