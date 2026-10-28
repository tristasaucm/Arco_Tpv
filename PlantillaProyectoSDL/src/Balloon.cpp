#include "rectangle.h"
#include "vector2D.h"
#include "Balloon.h"
#include "game.h"
#include <random>
#include "texture.h"
using namespace std;

Balloon::Balloon(Rectangle posAndDim, Vector2D<> speed, int color, Texture* texture):texture(texture), color(color) 
{
	this->posAndDim = posAndDim;
	this->speed = speed;
}
Balloon::~Balloon(){}

bool Balloon::hit(const Rectangle& obj) {
	
	if (posAndDim.hasIntersection(obj) && !animacion) 
	{
		animacion = true;
	}
	return animacion;
}
void Balloon::render() const
{
	// Si aún no ha sido explotado
	if (!animacion) 
	{
		texture->renderFrame(posAndDim.to_sdl(), color, 0);
	}
	// Animación de explosión
	else 
	{ 
		texture->renderFrame(posAndDim.to_sdl(), color, frame);
	}
}

bool Balloon::update(float delta)
{
	bool ret = false;
	// Movimiento
	posAndDim.move(speed *delta);

	// Si se ha salido de la pantalla
	if (posAndDim.getTopLeftCorner().getY() < 0 - posAndDim.getHeight()) 
	{
		fuerapantalla = true;
	}

	// Si ya le ha dado una flecha
	if (animacion) 
	{
		// Movimiento
		speed = Vector2D<>(0, 0.1f);
		// Cambio de frame para la animación de explosion
		if (SDL_GetTicks() - frameAnterior > 250.0f)
		{
			frameAnterior = (float)SDL_GetTicks();
			frame++;
		}
		// Si ha ejecutado la animacion de explosion completa devuelve true para que el game lo destruya
		if (animacion && frame >= texture->getNumColumns())
		{
			ret = true;
		}
	}
	return ret;
}

bool Balloon::fueraPantalla() const
{
	return fuerapantalla;
}

void Balloon::colocaGlobo(float Y) 
{
	posAndDim.move(Vector2D<>(0, Y));
	fuerapantalla = false;
}
