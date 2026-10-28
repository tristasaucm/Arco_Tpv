#pragma once
#include "game.h"
#include "GameObject.h"
class SceneObject : public GameObject
{
private:
	
public:
	Rectangle posAndDim;
	virtual bool hit(const Rectangle&);

};
