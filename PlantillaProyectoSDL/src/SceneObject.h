#pragma once
class Game;
#include "GameObject.h"
#include "rectangle.h"
#include "texture.h"
class SceneObject : public GameObject
{
private:
	
public:
	Rectangle posAndDim;
	Texture* texture;
	virtual bool hit(const Rectangle&);
	virtual void render() const = 0;
	virtual	void update() = 0;

};
