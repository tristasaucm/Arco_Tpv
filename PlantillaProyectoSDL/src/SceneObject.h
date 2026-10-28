#pragma once
#include "game.h"
#include "GameObject.h"
class SceneObject : public GameObject
{
private:
	
public:
	Rectangle posAndDim;
	Texture* texture;
	virtual bool hit(const Rectangle&);

};
