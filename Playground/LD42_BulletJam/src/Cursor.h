#ifndef _BULLETJAM_CURSOR_H_
#define _BULLETJAM_CURSOR_H_

#include <CherrySoda/Entity.h>

namespace ld42_bulletjam {

class Cursor : public cherrysoda::Entity
{
public:
	typedef cherrysoda::Entity base;

	void Update() override;

	static Cursor* Instance();

private:
	Cursor() = default;
	static Cursor* Create();
};

} // namespace ld42_bulletjam

#endif // _BULLETJAM_CURSOR_H_
