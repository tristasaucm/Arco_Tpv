#pragma once
#include "SceneObject.h"
class Bow : public SceneObject
{
private:
	Vector2D<> moveDir;
	int numArrows;
	// Indica si el arco esta cargado
	bool charged = false;
	bool mouseMoved = false;
	// Momento en el que se ha cargado la flecha
	float cargaArco = 0;
	float mouseY = 0;
public:
	Bow(Rectangle, int, Texture*, Game*);
	~Bow();
	void handleEvent(const SDL_Event& event);
	void render() const override;
	bool update() override;
	bool hit(const Rectangle&) override;

	// Devuelve el numero de flechas restantes
	int getNArrows() const;
	// Devuelve la posicion del centro del arco en el eje y
	float getYPos() const;
	// Cambia el numero de flechas
	void setNumArrows(int);
	// Devuelve si el arco esta cargado o no
	bool isCharged() const;
	float getChargeTime() const;
};

