#ifndef _CHERRYSODA_UTIL_TRACKER_H_
#define _CHERRYSODA_UTIL_TRACKER_H_

#include <CherrySoda/Util/NumType.h>
#include <CherrySoda/Util/STL.h>

namespace cherrysoda {

class Component;
class Entity;

// Per-scene registry of the live entities/components of tracked types, the
// C++ counterpart of Monocle's reflection-based Tracker. The engine has no
// RTTI, so types opt in manually: declare the type with
// CHERRYSODA_DECLARE_ENTITY / CHERRYSODA_DECLARE_COMPONENT and register it
// with CHERRYSODA_TRACK_ENTITY / CHERRYSODA_TRACK_COMPONENT in exactly one
// translation unit. Typed collision queries on Scene and Entity are built on
// top of these lists.
class Tracker
{
public:
	Tracker() = default;
	~Tracker() = default;

	Tracker(const Tracker&) = delete;
	Tracker& operator=(const Tracker&) = delete;

	void EntityAdded(Entity* entity);
	void EntityRemoved(Entity* entity);
	void ComponentAdded(Component* component);
	void ComponentRemoved(Component* component);

	// Returns nullptr when the type is not tracked
	const STL::List<Entity*>* GetEntitiesOfType(type::Int32 typeID) const;
	const STL::List<Component*>* GetComponentsOfType(type::Int32 typeID) const;

	int CountEntitiesOfType(type::Int32 typeID) const;
	int CountComponentsOfType(type::Int32 typeID) const;

	static void TrackEntityType(type::Int32 typeID);
	static void TrackComponentType(type::Int32 typeID);
	static bool IsEntityTypeTracked(type::Int32 typeID);
	static bool IsComponentTypeTracked(type::Int32 typeID);

private:
	static STL::HashSet<type::Int32>& GetTrackedEntityTypes();
	static STL::HashSet<type::Int32>& GetTrackedComponentTypes();

	STL::HashMap<type::Int32, STL::List<Entity*>> m_entities;
	STL::HashMap<type::Int32, STL::List<Component*>> m_components;
};

} // namespace cherrysoda

#define CHERRYSODA_TRACK_ENTITY(TYPE)                                                             \
	namespace {                                                                                   \
	struct TrackerRegistrar_##TYPE                                                                \
	{                                                                                             \
		TrackerRegistrar_##TYPE() { cherrysoda::Tracker::TrackEntityType(TYPE::EntityTypeID()); } \
	};                                                                                            \
	static TrackerRegistrar_##TYPE s_trackerRegistrar_##TYPE;                                     \
	}

#define CHERRYSODA_TRACK_COMPONENT(TYPE)                                                                         \
	namespace {                                                                                                  \
	struct ComponentTrackerRegistrar_##TYPE                                                                      \
	{                                                                                                            \
		ComponentTrackerRegistrar_##TYPE() { cherrysoda::Tracker::TrackComponentType(TYPE::ComponentTypeID()); } \
	};                                                                                                           \
	static ComponentTrackerRegistrar_##TYPE s_componentTrackerRegistrar_##TYPE;                                  \
	}

#endif // _CHERRYSODA_UTIL_TRACKER_H_
