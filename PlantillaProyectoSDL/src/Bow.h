#pragma once
#include "rectangle.h"
#include "vector2D.h"
#include "texture.h"
class Game;

class Bow
{
private:
	Rectangle posAndDim = Rectangle();
	Vector2D<> moveDir = Vector2D();
	int numArrows;
	Texture* texture;
	// Indica si el arco esta cargado
	bool charged = false;
public:
	Bow(Rectangle, int, Texture*);
	~Bow();
	void handleEvent(const SDL_Event& event);
	void render() const;
	bool update(float delta);

	// Cambia el estado del arco si se ha podido disparar
	bool Shoot();
	// Devuelve el numero de flechas restantes
	int getNArrows() const;
	// Devuelve la posicion del centro del arco en el eje y
	float getYPos() const;
	// Cambia el numero de flechas
	void setNumArrows(int);
	// Devuelve si el arco esta cargado o no
	bool isCharged() const;
};

