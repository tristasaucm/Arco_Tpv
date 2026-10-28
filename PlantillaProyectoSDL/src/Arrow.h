#pragma once
#include "rectangle.h"
#include "vector2D.h"
#include "texture.h"
#include "MobileObject.h"
class Game;

class Arrow : public MobileObject
{
private:
	
public:
	Arrow(Rectangle, Vector2D<>, Texture*, Game*);
	~Arrow();
	void render() const override;
};
