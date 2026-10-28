#pragma once
#include "game.h"
#include "SceneObject.h"
class MobileObject : public SceneObject
{
private:
	
public:
	Vector2D<> speed;
	void update() override;

};
