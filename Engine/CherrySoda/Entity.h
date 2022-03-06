#ifndef _CHERRYSODA_ENTITY_H_
#define _CHERRYSODA_ENTITY_H_

#include <CherrySoda/InternalUtilities/ComponentList.h>

#include <CherrySoda/Util/BitTag.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/Pool.h>
#include <CherrySoda/Util/STL.h>

namespace cherrysoda {

class Camera;
class CollidableComponent;
class Collider;
class Component;
class Scene;

class Entity
{
public:
	Entity() : Entity(Math::Vec3(0.f)) {}
	Entity(const Math::Vec2& position) : Entity(Math::Vec3(position, 0.f)) {}
	Entity(const Math::Vec3& position);
	virtual ~Entity();

	CHERRYSODA_GETTER_SETTER_OF_VEC3(Position, m_position);
	CHERRYSODA_GETTER_SETTER_OF_BOOL(Active, m_active);
	CHERRYSODA_GETTER_SETTER_OF_BOOL(Visible, m_visible);
	CHERRYSODA_GETTER_SETTER_OF_BOOL(Collidable, m_collidable);

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

	inline BitTagValueType Tag() const { return m_tag; }
	void Tag(BitTagValueType tag);

	const STL::List<Entity*> CollideAll(const BitTag& tag) const;
	bool CollideCheck(const BitTag& tag) const;
	bool CollideCheck(const BitTag& tag, const Math::Vec2& at);
	bool CollideCheck(const Entity* other) const;
	bool CollideCheck(const Entity* other, const Math::Vec2& at);
	bool CollideCheck(const CollidableComponent* other) const;
	bool CollideCheck(const CollidableComponent* other, const Math::Vec2& at);
	bool CollidePoint(const Math::Vec2& point) const;
	bool CollideLine(const Math::Vec2& from, const Math::Vec2& to) const;
	int CollideCount(const BitTag& tag) const;
	Entity* CollideFirst(const BitTag& tag) const;

	float Left() const;
	float Right() const;
	float Bottom() const;
	float Top() const;
	void Left(float left);
	void Right(float right);
	void Bottom(float bottom);
	void Top(float top);

	void Add(Component* component);
	void Remove(Component* component);
	void Add(const ComponentList::IterableComponents& components);
	void Remove(const ComponentList::IterableComponents& components);

	void RemoveAllComponents();

	void AutoDeleteWhenRemoved() { if (m_onRemoved == nullptr) m_onRemoved = Entity::CleanAndDeleteEntity; }
	void AutoDeleteAllInsideWhenRemoved();
	void CancleAutoDelete() { m_onRemoved = nullptr; }

	void Depth(int depth);
	inline int Depth() const { return m_depth; }

	template<class T>
	T* Get() { return m_components != nullptr ? m_components->Get<T>() : nullptr; }

	inline Scene* GetScene() { return m_scene; }
	template <typename T>
	inline T* GetSceneAs() const { return static_cast<T*>(m_scene); }

	inline Collider* GetCollider() const { return m_collider; }
	template <typename T>
	inline T* GetColliderAs() const { return static_cast<T*>(m_collider); }
	void SetCollider(Collider* collider);

private:
	CHERRYSODA_FRIEND_CLASS_POOL;

	friend class EntityList;
	friend class Scene;

	void AutoDeleteWhenRemoved(PoolInterface* pool);

	static void CleanAndDeleteEntity(Entity* entity, Scene* scene);

	virtual void Added(Scene* scene);
	virtual void Removed(Scene* scene);

	Math::Vec3 m_position = Math::Vec3(0.f);
	bool m_active = true;
	bool m_visible = true;
	bool m_collidable = true;

	Scene* m_scene = nullptr;
	ComponentList* m_components = nullptr;

	int m_id = 0;
	BitTagValueType m_tag = 0;

	Collider* m_collider = nullptr;

	int m_depth = 0;
	double m_actualDepth = 0.0;

	STL::Action<Entity*, Scene*> m_onRemoved;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_ENTITY_H_
