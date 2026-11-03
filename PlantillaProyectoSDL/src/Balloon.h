#pragma once
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

	void render() const override;
	bool hit(const Rectangle&) override;
	void update() override;
};
