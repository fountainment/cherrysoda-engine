#ifndef _CHERRYSODA_SCENE_H_
#define _CHERRYSODA_SCENE_H_

#include <CherrySoda/Colliders/Collide.h>
#include <CherrySoda/Components/CollidableComponent.h>
#include <CherrySoda/Engine.h>
#include <CherrySoda/Entity.h>
#include <CherrySoda/Util/BitTag.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/STL.h>
#include <CherrySoda/Util/Tracker.h>

namespace cherrysoda {

class EntityList;
class Renderer;
class RendererList;
class TagLists;

class Scene
{
public:
	Scene();
	virtual ~Scene();

	virtual void Begin();
	virtual void End();

	virtual void BeforeUpdate();
	virtual void Update();
	virtual void AfterUpdate();

	virtual void BeforeRender();
	virtual void Render();
	virtual void AfterRender();

	virtual void HandleGraphicsReset();
	virtual void HandleGraphicsCreate();

	virtual void GainFocus();
	virtual void LoseFocus();

	void Add(Entity* entity);
	void Remove(Entity* entity);

	const STL::List<Entity*> GetEntitiesByTagMask(BitTagValueType mask) const;
	const STL::List<Entity*> GetEntitiesExcludingTagMask(BitTagValueType mask) const;

	void Add(Renderer* renderer);
	void Remove(Renderer* renderer);

	void INTERNAL_SetActualDepth(Entity* entity);

	inline EntityList* Entities() { return m_entities; }
	inline TagLists* Tags() { return m_tagLists; }
	inline RendererList* Renderers() { return m_rendererList; }
	Tracker* GetTracker() const { return m_tracker; }

	Renderer* FirstRenderer();

	inline Entity* HelperEntity() { return m_helperEntity; }

	inline float TimeActive() const { return m_timeActive; }
	inline float RawTimeActive() const { return m_rawTimeActive; }

	inline bool OnInterval(float interval)
	{
		return (int)((TimeActive() - Engine::Instance()->DeltaTime()) / interval) < (int)(TimeActive() / interval);
	}

	inline bool OnInterval(float interval, float offset)
	{
		return Math_Floor((TimeActive() - offset - Engine::Instance()->DeltaTime()) / interval) <
			   Math_Floor((TimeActive() - offset) / interval);
	}

	inline bool BetweenInterval(float interval) { return Math_Mod(TimeActive(), interval * 2) > interval; }

	inline bool BetweenInterval(float interval, float offset)
	{
		return Math_Mod(TimeActive() - offset, interval * 2) > interval;
	}

	inline bool OnRawInterval(float interval)
	{
		return (int)((RawTimeActive() - Engine::Instance()->RawDeltaTime()) / interval) <
			   (int)(RawTimeActive() / interval);
	}

	inline bool OnRawInterval(float interval, float offset)
	{
		return Math_Floor((RawTimeActive() - offset - Engine::Instance()->RawDeltaTime()) / interval) <
			   Math_Floor((RawTimeActive() - offset) / interval);
	}

	inline bool BetweenRawInterval(float interval) { return Math_Mod(RawTimeActive(), interval * 2) > interval; }

	inline bool BetweenRawInterval(float interval, float offset)
	{
		return Math_Mod(RawTimeActive() - offset, interval * 2) > interval;
	}

	bool CollideCheck(const Math::Vec2& point, int tag) const;
	bool CollideCheck(const Math::Vec2& from, const Math::Vec2& to, int tag) const;
	bool CollideCheck(const Math::Rectangle& rect, int tag) const;
	Math::Vec2 LineWalkCheck(const Math::Vec2& from, const Math::Vec2& to, int tag, float precision) const;

	Entity* CollideFirst(const Math::Vec2& point, int tag) const;
	Entity* CollideFirst(const Math::Vec2& from, const Math::Vec2& to, int tag) const;
	Entity* CollideFirst(const Math::Rectangle& rect, int tag) const;

	const STL::List<Entity*> CollideAll(const Math::Vec2& point, int tag) const;
	const STL::List<Entity*> CollideAll(const Math::Vec2& from, const Math::Vec2& to, int tag) const;
	const STL::List<Entity*> CollideAll(const Math::Rectangle& rect, int tag) const;

	// Typed queries over tracked entity types (CHERRYSODA_TRACK_ENTITY);
	// untracked types simply report no hits
	template<class T> const STL::List<Entity*>* GetTrackedEntities() const
	{
		static_assert(std::is_base_of<Entity, T>::value, "T must derive from Entity");
		return GetTracker()->GetEntitiesOfType(T::EntityTypeID());
	}

	// Typed queries over tracked CollidableComponent types
	// (CHERRYSODA_TRACK_COMPONENT)
	template<class T> const STL::List<Component*>* GetTrackedComponents() const
	{
		static_assert(std::is_base_of<CollidableComponent, T>::value, "T must derive from CollidableComponent");
		return GetTracker()->GetComponentsOfType(T::ComponentTypeID());
	}

	template<class T> bool CollideCheck(const Math::Vec2& point) const
	{
		auto list = GetTrackedEntities<T>();
		if (list == nullptr) {
			return false;
		}
		for (auto entity : *list) {
			if (entity->Collidable() && entity->CollidePoint(point)) {
				return true;
			}
		}
		return false;
	}

	template<class T> bool CollideCheck(const Math::Vec2& from, const Math::Vec2& to) const
	{
		auto list = GetTrackedEntities<T>();
		if (list == nullptr) {
			return false;
		}
		for (auto entity : *list) {
			if (entity->Collidable() && entity->CollideLine(from, to)) {
				return true;
			}
		}
		return false;
	}

	template<class T> bool CollideCheck(const Math::Rectangle& rect) const
	{
		auto list = GetTrackedEntities<T>();
		if (list == nullptr) {
			return false;
		}
		for (auto entity : *list) {
			if (entity->Collidable() && Collide::CheckRect(entity, rect)) {
				return true;
			}
		}
		return false;
	}

	template<class T> T* CollideFirst(const Math::Vec2& point) const
	{
		auto list = GetTrackedEntities<T>();
		if (list != nullptr) {
			for (auto entity : *list) {
				if (entity->Collidable() && entity->CollidePoint(point)) {
					return static_cast<T*>(entity);
				}
			}
		}
		return nullptr;
	}

	template<class T> T* CollideFirst(const Math::Vec2& from, const Math::Vec2& to) const
	{
		auto list = GetTrackedEntities<T>();
		if (list != nullptr) {
			for (auto entity : *list) {
				if (entity->Collidable() && entity->CollideLine(from, to)) {
					return static_cast<T*>(entity);
				}
			}
		}
		return nullptr;
	}

	template<class T> T* CollideFirst(const Math::Rectangle& rect) const
	{
		auto list = GetTrackedEntities<T>();
		if (list != nullptr) {
			for (auto entity : *list) {
				if (entity->Collidable() && Collide::CheckRect(entity, rect)) {
					return static_cast<T*>(entity);
				}
			}
		}
		return nullptr;
	}

	template<class T> const STL::List<T*> CollideAll(const Math::Vec2& point) const
	{
		STL::List<T*> hits;
		auto list = GetTrackedEntities<T>();
		if (list != nullptr) {
			for (auto entity : *list) {
				if (entity->Collidable() && entity->CollidePoint(point)) {
					STL::Add(hits, static_cast<T*>(entity));
				}
			}
		}
		return hits;
	}

	template<class T> const STL::List<T*> CollideAll(const Math::Vec2& from, const Math::Vec2& to) const
	{
		STL::List<T*> hits;
		auto list = GetTrackedEntities<T>();
		if (list != nullptr) {
			for (auto entity : *list) {
				if (entity->Collidable() && entity->CollideLine(from, to)) {
					STL::Add(hits, static_cast<T*>(entity));
				}
			}
		}
		return hits;
	}

	template<class T> const STL::List<T*> CollideAll(const Math::Rectangle& rect) const
	{
		STL::List<T*> hits;
		auto list = GetTrackedEntities<T>();
		if (list != nullptr) {
			for (auto entity : *list) {
				if (entity->Collidable() && Collide::CheckRect(entity, rect)) {
					STL::Add(hits, static_cast<T*>(entity));
				}
			}
		}
		return hits;
	}

	template<class T> bool CollideCheckByComponent(const Math::Vec2& point) const
	{
		auto list = GetTrackedComponents<T>();
		if (list == nullptr) {
			return false;
		}
		for (auto component : *list) {
			if (ComponentCanCollide(component) && static_cast<T*>(component)->GetCollider()->Collide(point)) {
				return true;
			}
		}
		return false;
	}

	template<class T> bool CollideCheckByComponent(const Math::Vec2& from, const Math::Vec2& to) const
	{
		auto list = GetTrackedComponents<T>();
		if (list == nullptr) {
			return false;
		}
		for (auto component : *list) {
			if (ComponentCanCollide(component) && static_cast<T*>(component)->GetCollider()->Collide(from, to)) {
				return true;
			}
		}
		return false;
	}

	template<class T> bool CollideCheckByComponent(const Math::Rectangle& rect) const
	{
		auto list = GetTrackedComponents<T>();
		if (list == nullptr) {
			return false;
		}
		for (auto component : *list) {
			if (ComponentCanCollide(component) && static_cast<T*>(component)->GetCollider()->Collide(rect)) {
				return true;
			}
		}
		return false;
	}

	template<class T> T* CollideFirstByComponent(const Math::Vec2& point) const
	{
		auto list = GetTrackedComponents<T>();
		if (list != nullptr) {
			for (auto component : *list) {
				if (ComponentCanCollide(component) && static_cast<T*>(component)->GetCollider()->Collide(point)) {
					return static_cast<T*>(component);
				}
			}
		}
		return nullptr;
	}

	template<class T> T* CollideFirstByComponent(const Math::Vec2& from, const Math::Vec2& to) const
	{
		auto list = GetTrackedComponents<T>();
		if (list != nullptr) {
			for (auto component : *list) {
				if (ComponentCanCollide(component) && static_cast<T*>(component)->GetCollider()->Collide(from, to)) {
					return static_cast<T*>(component);
				}
			}
		}
		return nullptr;
	}

	template<class T> T* CollideFirstByComponent(const Math::Rectangle& rect) const
	{
		auto list = GetTrackedComponents<T>();
		if (list != nullptr) {
			for (auto component : *list) {
				if (ComponentCanCollide(component) && static_cast<T*>(component)->GetCollider()->Collide(rect)) {
					return static_cast<T*>(component);
				}
			}
		}
		return nullptr;
	}

	template<class T> const STL::List<T*> CollideAllByComponent(const Math::Vec2& point) const
	{
		STL::List<T*> hits;
		auto list = GetTrackedComponents<T>();
		if (list != nullptr) {
			for (auto component : *list) {
				if (ComponentCanCollide(component) && static_cast<T*>(component)->GetCollider()->Collide(point)) {
					STL::Add(hits, static_cast<T*>(component));
				}
			}
		}
		return hits;
	}

	template<class T> const STL::List<T*> CollideAllByComponent(const Math::Vec2& from, const Math::Vec2& to) const
	{
		STL::List<T*> hits;
		auto list = GetTrackedComponents<T>();
		if (list != nullptr) {
			for (auto component : *list) {
				if (ComponentCanCollide(component) && static_cast<T*>(component)->GetCollider()->Collide(from, to)) {
					STL::Add(hits, static_cast<T*>(component));
				}
			}
		}
		return hits;
	}

	template<class T> const STL::List<T*> CollideAllByComponent(const Math::Rectangle& rect) const
	{
		STL::List<T*> hits;
		auto list = GetTrackedComponents<T>();
		if (list != nullptr) {
			for (auto component : *list) {
				if (ComponentCanCollide(component) && static_cast<T*>(component)->GetCollider()->Collide(rect)) {
					STL::Add(hits, static_cast<T*>(component));
				}
			}
		}
		return hits;
	}

	const STL::List<Entity*>& operator[](const BitTag& tag) const;
	const STL::List<Entity*>& Get(const BitTag& tag) const;

	void AddActionOnEndOfFrame(STL::Action<> func);

private:
	static bool ComponentCanCollide(const Component* component);

	EntityList* m_entities = nullptr;
	TagLists* m_tagLists = nullptr;
	RendererList* m_rendererList = nullptr;
	Tracker* m_tracker = nullptr;

	Entity* m_helperEntity = nullptr;

	float m_timeActive = 0.f;
	float m_rawTimeActive = 0.f;

	bool m_focused = false;
	bool m_paused = false;

	STL::Vector<STL::Action<>> m_onEndOfFrame;
	STL::HashMap<int, double> m_actualDepthLookup;
};

// Definitions of Entity's typed queries; they live here because they need
// both Entity and Scene complete
template<class T> inline bool Entity::CollideCheck() const
{
	CHERRYSODA_ASSERT(m_scene != nullptr,
					  "Can't collide check an Entity against a tracked type when it is not a member of a Scene\n");
	auto list = m_scene->GetTrackedEntities<T>();
	if (list == nullptr) {
		return false;
	}
	for (auto entity : *list) {
		if (CollideCheck(entity)) {
			return true;
		}
	}
	return false;
}

template<class T> inline bool Entity::CollideCheck(const Math::Vec2& at)
{
	Math::Vec2 position = Position2D();
	Position2D(at);
	bool result = CollideCheck<T>();
	Position2D(position);
	return result;
}

template<class T> inline T* Entity::CollideFirst() const
{
	CHERRYSODA_ASSERT(m_scene != nullptr,
					  "Can't collide check an Entity against a tracked type when it is not a member of a Scene\n");
	auto list = m_scene->GetTrackedEntities<T>();
	if (list != nullptr) {
		for (auto entity : *list) {
			if (CollideCheck(entity)) {
				return static_cast<T*>(entity);
			}
		}
	}
	return nullptr;
}

template<class T> inline const STL::List<T*> Entity::CollideAll() const
{
	CHERRYSODA_ASSERT(m_scene != nullptr,
					  "Can't collide check an Entity against a tracked type when it is not a member of a Scene\n");
	STL::List<T*> hits;
	auto list = m_scene->GetTrackedEntities<T>();
	if (list != nullptr) {
		for (auto entity : *list) {
			if (CollideCheck(entity)) {
				STL::Add(hits, static_cast<T*>(entity));
			}
		}
	}
	return hits;
}

template<class T> inline void Entity::CollideDo(const STL::Action<T*>& action) const
{
	CHERRYSODA_ASSERT(m_scene != nullptr,
					  "Can't collide check an Entity against a tracked type when it is not a member of a Scene\n");
	auto list = m_scene->GetTrackedEntities<T>();
	if (list != nullptr && action != nullptr) {
		for (auto entity : *list) {
			if (CollideCheck(entity)) {
				action(static_cast<T*>(entity));
			}
		}
	}
}

template<class T> inline bool Entity::CollideCheckByComponent() const
{
	CHERRYSODA_ASSERT(
		m_scene != nullptr,
		"Can't collide check an Entity against a tracked component type when it is not a member of a Scene\n");
	auto list = m_scene->GetTrackedComponents<T>();
	if (list == nullptr) {
		return false;
	}
	for (auto component : *list) {
		if (CollideCheck(static_cast<T*>(component))) {
			return true;
		}
	}
	return false;
}

template<class T> inline bool Entity::CollideCheckByComponent(const Math::Vec2& at)
{
	Math::Vec2 position = Position2D();
	Position2D(at);
	bool result = CollideCheckByComponent<T>();
	Position2D(position);
	return result;
}

template<class T> inline T* Entity::CollideFirstByComponent() const
{
	CHERRYSODA_ASSERT(
		m_scene != nullptr,
		"Can't collide check an Entity against a tracked component type when it is not a member of a Scene\n");
	auto list = m_scene->GetTrackedComponents<T>();
	if (list != nullptr) {
		for (auto component : *list) {
			if (CollideCheck(static_cast<T*>(component))) {
				return static_cast<T*>(component);
			}
		}
	}
	return nullptr;
}

template<class T> inline const STL::List<T*> Entity::CollideAllByComponent() const
{
	CHERRYSODA_ASSERT(
		m_scene != nullptr,
		"Can't collide check an Entity against a tracked component type when it is not a member of a Scene\n");
	STL::List<T*> hits;
	auto list = m_scene->GetTrackedComponents<T>();
	if (list != nullptr) {
		for (auto component : *list) {
			if (CollideCheck(static_cast<T*>(component))) {
				STL::Add(hits, static_cast<T*>(component));
			}
		}
	}
	return hits;
}

template<class T> inline void Entity::CollideDoByComponent(const STL::Action<T*>& action) const
{
	CHERRYSODA_ASSERT(
		m_scene != nullptr,
		"Can't collide check an Entity against a tracked component type when it is not a member of a Scene\n");
	auto list = m_scene->GetTrackedComponents<T>();
	if (list != nullptr && action != nullptr) {
		for (auto component : *list) {
			if (CollideCheck(static_cast<T*>(component))) {
				action(static_cast<T*>(component));
			}
		}
	}
}

} // namespace cherrysoda

#endif // _CHERRYSODA_SCENE_H_
