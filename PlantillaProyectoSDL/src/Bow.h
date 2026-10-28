#pragma once
#include "rectangle.h"
#include "vector2D.h"
#include "texture.h"
#include "SceneObject.h"
class Game;

class Bow : public SceneObject
{
private:
	Vector2D<> moveDir;
	int numArrows;
	// Indica si el arco esta cargado
	bool charged = false;
public:
	Bow(Rectangle, int, Texture*, Game*);
	~Bow();
	void handleEvent(const SDL_Event& event);
	void render() const override;
	void update() override;

	// Devuelve el numero de flechas restantes
	int getNArrows() const;
	// Devuelve la posicion del centro del arco en el eje y
	float getYPos() const;
	// Cambia el numero de flechas
	void setNumArrows(int);
	// Devuelve si el arco esta cargado o no
	bool isCharged() const;
};

