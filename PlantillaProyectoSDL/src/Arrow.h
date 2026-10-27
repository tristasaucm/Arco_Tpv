#pragma once
#include "rectangle.h"
#include "vector2D.h"
#include "texture.h"
class Game;

class Arrow
{
private:
	Rectangle posAndDim;
	Vector2D<> speed;
	Texture* texture;
	Game* game;
	// Indica si ha sido disparada
	bool shoot;
public:
	Arrow(Rectangle, Vector2D<>, Texture*, Game*);
	~Arrow();
	void render() const;
	// Devuelve true cuando se ha salido de la pantalla
	bool update(float delta);
	// Permite que la flecha se mueva hacia la derecha
	void dispara();
};
