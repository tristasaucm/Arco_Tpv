#pragma once
#include "GameObject.h"
#include "rectangle.h"
#include "texture.h"
class SceneObject : public GameObject
{
protected:
	Rectangle posAndDim;
	Texture* texture;

public:
	virtual bool hit(const Rectangle&);

};
