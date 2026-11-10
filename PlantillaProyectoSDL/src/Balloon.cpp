#include "Balloon.h"
#include "game.h"
using namespace std;

Balloon::Balloon(Rectangle posAndDim, Vector2D<> speed, int color, Texture* texture, Game* game): color(color) 
{
	this->texture = texture;
	this->posAndDim = posAndDim;
	this->speed = speed;
	this->game = game;
}

void Balloon::avanzaFrame() 
{
	frame++;
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

// Devuelve true si se sale de la pantalla
bool Balloon::update(float dt) 
{
	bool ret = MobileObject::update(dt);
	if (ret) game->reduceNumGlobos();
	if (animacion && (SDL_GetTicks() - frameAnterior) > 100) 
	{
		if (speed.getY() < 0) speed = speed * -1;
		frame++;
		frameAnterior = (float)SDL_GetTicks();
	}
	return ret;
}
bool Balloon::hit(const Rectangle& obj)
{
	if (!animacion) 
	{
		animacion = posAndDim.hasIntersection(obj);
	}
	else if (frame > texture->getNumColumns())
	{
		game->reduceNumGlobos();
		return true;
	}
	return false;
}
