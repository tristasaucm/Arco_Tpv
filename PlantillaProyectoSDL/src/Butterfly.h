#pragma once
#include "MobileObject.h"
class Butterfly : public MobileObject
{
public:
	Butterfly(Rectangle, Vector2D<>, Texture*, Game*);
	bool update(float dt) override;

private:

};
