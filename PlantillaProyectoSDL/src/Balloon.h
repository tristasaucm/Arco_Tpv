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
	void avanzaFrame();
public:
	Balloon(Rectangle, Vector2D<>, int, Texture*, Game*);

	void render() const override;
	// Devuelve true si se sale de la pantalla
	bool update(float dt) override;
	bool hit(const Rectangle&) override;
};
