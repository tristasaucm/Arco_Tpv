#pragma once
#include "SceneObject.h"
class MobileObject : public SceneObject
{
protected:
	Vector2D<> speed;
	
public:
	// Devuelve true si se sale de la pantalla
	bool update(float dt) override;
};
