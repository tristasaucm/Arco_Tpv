#pragma once
#include "rectangle.h"
#include "vector2D.h"
#include "texture.h"
#include "MobileObject.h"
class Balloon : public MobileObject
{
private:
	Texture* texture;
	int color;
	// Indica si hay que ejecutar la animacion de explosion
	bool animacion = false;
	// Indica si se ha salido de la pantalla
	bool fuerapantalla = false;
	// Momento en el que se ha puesto la textura anterior
	float frameAnterior = 0;
	// Frame a renderizar
	int frame = 0;
public:
	Balloon(Rectangle, Vector2D<>, int, Texture*);
	~Balloon();
	bool hit(const Rectangle&);

	void render() const override;
	// Devuelve true si el globo se ha salido de la pantalla
	bool fueraPantalla() const;
	// Coloca el globo en la posicion en el eje Y
	void colocaGlobo(float);
};


