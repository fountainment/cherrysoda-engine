#ifndef _CHERRYSODA_SCENE_H_
#define _CHERRYSODA_SCENE_H_

#include <CherrySoda/Engine.h>
#include <CherrySoda/Util/BitTag.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/STL.h>

namespace cherrysoda {

class Entity;
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

	Renderer* FirstRenderer();

	inline Entity* HelperEntity() { return m_helperEntity; }

	inline float TimeActive() const { return m_timeActive; }
	inline float RawTimeActive() const { return m_rawTimeActive; }

	inline bool OnInterval(float interval) {
		return (int)((TimeActive() - Engine::Instance()->DeltaTime()) / interval) < (int)(TimeActive() / interval);
	}

	bool CollideCheck(const Math::Vec2& point, int tag);
	Math::Vec2 LineWalkCheck(const Math::Vec2& from, const Math::Vec2& to, int tag, float precision);

	const STL::List<Entity*>& operator [] (const BitTag& tag) const;

	void AddActionOnEndOfFrame(STL::Action<> func);

private:
	EntityList* m_entities = nullptr;
	TagLists* m_tagLists = nullptr;
	RendererList* m_rendererList = nullptr;

	Entity* m_helperEntity = nullptr;

	float m_timeActive = 0.f;
	float m_rawTimeActive = 0.f;

	bool m_focused = false;
	bool m_paused = false;

	STL::Vector<STL::Action<>> m_onEndOfFrame;
	STL::HashMap<int,double> m_actualDepthLookup;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_SCENE_H_
