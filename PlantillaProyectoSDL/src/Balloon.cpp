#include "Balloon.h"
using namespace std;

Balloon::Balloon(Rectangle posAndDim, Vector2D<> speed, int color, Texture* texture):texture(texture), color(color) 
{
	this->posAndDim = posAndDim;
	this->speed = speed;
}
Balloon::~Balloon(){}

bool Balloon::hit(const Rectangle& obj) 
{	
	if (posAndDim.hasIntersection(obj) && !animacion) 
	{
		animacion = true;
		return true;
	}
	else return false;
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
