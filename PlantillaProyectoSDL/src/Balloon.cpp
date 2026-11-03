#include "Balloon.h"
using namespace std;

Balloon::Balloon(Rectangle posAndDim, Vector2D<> speed, int color, Texture* texture): color(color) 
{
	this->texture = texture;
	this->posAndDim = posAndDim;
	this->speed = speed;
}
Balloon::~Balloon(){}

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
void Balloon::update() {
	if (animacion) {
		frame++;
	}
	posAndDim.move(speed);
	
}
bool Balloon::hit(const Rectangle& obj)
{
	if (!animacion) {
		return posAndDim.hasIntersection(obj);
	}
	else {
		return false;
	}
}
