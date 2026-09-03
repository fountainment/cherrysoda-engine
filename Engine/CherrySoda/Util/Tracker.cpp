#include <CherrySoda/Util/Tracker.h>

#include <CherrySoda/Components/Component.h>
#include <CherrySoda/Entity.h>
#include <CherrySoda/Util/STL.h>

namespace cherrysoda {

STL::HashSet<type::Int32>& Tracker::GetTrackedEntityTypes()
{
	static STL::HashSet<type::Int32> trackedEntityTypes;
	return trackedEntityTypes;
}

STL::HashSet<type::Int32>& Tracker::GetTrackedComponentTypes()
{
	static STL::HashSet<type::Int32> trackedComponentTypes;
	return trackedComponentTypes;
}

void Tracker::EntityAdded(Entity* entity)
{
	if (entity != nullptr && IsEntityTypeTracked(entity->TypeID())) {
		m_entities[entity->TypeID()].push_back(entity);
	}
}

void Tracker::EntityRemoved(Entity* entity)
{
	if (entity == nullptr) {
		return;
	}
	auto it = m_entities.find(entity->TypeID());
	if (it != m_entities.end()) {
		STL::Remove(it->second, entity);
	}
}

void Tracker::ComponentAdded(Component* component)
{
	if (component != nullptr && IsComponentTypeTracked(component->TypeID())) {
		m_components[component->TypeID()].push_back(component);
	}
}

void Tracker::ComponentRemoved(Component* component)
{
	if (component == nullptr) {
		return;
	}
	auto it = m_components.find(component->TypeID());
	if (it != m_components.end()) {
		STL::Remove(it->second, component);
	}
}

const STL::List<Entity*>* Tracker::GetEntitiesOfType(type::Int32 typeID) const
{
	auto it = m_entities.find(typeID);
	return it != m_entities.end() ? &it->second : nullptr;
}

const STL::List<Component*>* Tracker::GetComponentsOfType(type::Int32 typeID) const
{
	auto it = m_components.find(typeID);
	return it != m_components.end() ? &it->second : nullptr;
}

int Tracker::CountEntitiesOfType(type::Int32 typeID) const
{
	auto list = GetEntitiesOfType(typeID);
	return list != nullptr ? static_cast<int>(STL::Count(*list)) : 0;
}

int Tracker::CountComponentsOfType(type::Int32 typeID) const
{
	auto list = GetComponentsOfType(typeID);
	return list != nullptr ? static_cast<int>(STL::Count(*list)) : 0;
}

void Tracker::TrackEntityType(type::Int32 typeID)
{
	STL::Add(GetTrackedEntityTypes(), typeID);
}

void Tracker::TrackComponentType(type::Int32 typeID)
{
	STL::Add(GetTrackedComponentTypes(), typeID);
}

bool Tracker::IsEntityTypeTracked(type::Int32 typeID)
{
	return STL::Contains(GetTrackedEntityTypes(), typeID);
}

bool Tracker::IsComponentTypeTracked(type::Int32 typeID)
{
	return STL::Contains(GetTrackedComponentTypes(), typeID);
}

} // namespace cherrysoda
