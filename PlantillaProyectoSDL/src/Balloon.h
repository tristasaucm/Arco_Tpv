#pragma once
#include "rectangle.h"
#include "vector2D.h"
#include "texture.h"
#include "MobileObject.h"
class Balloon : public MobileObject
{
private:
	int color;
	// Indica si hay que ejecutar la animacion de explosion
	bool animacion = false;
	// Momento en el que se ha puesto la textura anterior
	float frameAnterior = 0;
	// Frame a renderizar
	int frame = 0;
public:
	Balloon(Rectangle, Vector2D<>, int, Texture*);
	~Balloon();
	bool hit(const Rectangle&);

	void render() const override;
};
