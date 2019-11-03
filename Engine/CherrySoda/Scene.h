#ifndef _CHERRYSODA_SCENE_H_
#define _CHERRYSODA_SCENE_H_

#include <CherrySoda/InternalUtilities/EntityList.h>

#include <functional>
#include <vector>

namespace cherrysoda {

class Scene
{
public:
	Scene();

	virtual void Begin();
	virtual void End(); 

	virtual void BeforeUpdate();	
	virtual void Update();	
	virtual void AfterUpdate();	

	virtual void BeforeRender();
	virtual void Render();
	virtual void AfterRender();

	EntityList* Entities() { return m_entities; }

	void OnEndOfFrame(std::function<void()> func);

private:
	EntityList* m_entities = nullptr;

	bool m_focused = false;
	bool m_paused = false;

	std::vector<std::function<void()>> m_onEndOfFrame;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_SCENE_H_