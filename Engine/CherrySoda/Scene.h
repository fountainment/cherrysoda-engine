#ifndef _CHERRYSODA_SCENE_H_
#define _CHERRYSODA_SCENE_H_

#include <CherrySoda/InternalUtilities/EntityList.h>

namespace cherrysoda {

class Scene
{
public:
	virtual void Begin();
	virtual void End(); 

	virtual void BeforeUpdate();	
	virtual void Update();	
	virtual void AfterUpdate();	

	virtual void BeforeRender();
	virtual void Render();
	virtual void AfterRender();

	EntityList* Entities() { return &m_entities; }

private:
	EntityList m_entities;

	bool m_focused = false;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_SCENE_H_