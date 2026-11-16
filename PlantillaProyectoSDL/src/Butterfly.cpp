#include "Butterfly.h"
#include "game.h"
using namespace std;

random_device rd1;
mt19937 gen1(rd1());
uniform_real_distribution<float> velRdB(2,4);
uniform_real_distribution<float> velRdS(-4,4);

Butterfly::Butterfly(Rectangle posAndDim, Vector2D<> speed, Texture* texture, Game* game) 
{
	this->texture = texture;
	this->posAndDim = posAndDim;
	this->speed = speed;
	this->game = game;
}
bool Butterfly::update(float dt)
{
	posAndDim.move(speed);
	
	// Si se sale de la pantalla
	bool saleDrch = posAndDim.getTopLeftCorner().getX() > game->WINDOW_WIDTH - posAndDim.getWidth();
	bool saleIzq = posAndDim.getTopLeftCorner().getX() < 0;
	bool saleAbajo = posAndDim.getTopLeftCorner().getY() > game->WINDOW_HEIGHT - posAndDim.getHeight();
	bool saleArriba = posAndDim.getTopLeftCorner().getY() < 0;

	//Cambiamos la velocidad si se sale de la pantalla
	if (saleDrch && speed.getX() > 0) 
	{
		animation = 2;
		frameAnimation = 0;
		speed = Point2D<>(-velRdB(gen1), velRdS(gen1));
	}
	else if (saleIzq && speed.getX() < 0)
	{
		animation = 3;
		frameAnimation = 0;
		speed = Point2D<>(velRdB(gen1), velRdS(gen1));
	}

	if (saleArriba && speed.getY() < 0)
	{
		float newSpeedX = velRdS(gen1);
		if (speed.getX() < 0 && newSpeedX > 0)
		{
			animation = 3;
			frameAnimation = 0;
		}
		else if (speed.getX() > 0 && newSpeedX < 0)
		{
			animation = 2;
			frameAnimation = 0;
		}
		speed = Point2D<>(newSpeedX, velRdB(gen1));
	}
	else if (saleAbajo && speed.getY() > 0)
	{
		float newSpeedX = velRdS(gen1);
		if (speed.getX() < 0 && newSpeedX > 0)
		{
			animation = 3;
			frameAnimation = 0;
		}
		else if (speed.getX() > 0 && newSpeedX < 0)
		{
			animation = 2;
			frameAnimation = 0;
		}
		speed = Point2D<>(newSpeedX, -velRdB(gen1));
	}
	// Animacion
	if (animation != 2 && animation != 3) 
	{
		if (speed.getX() > 0) animation = 1;
		else animation = 0;
	}
	if (frameAnimation < texture->getNumColumns() - 1) frameAnimation++;
	else
	{
		if (animation = 2) animation = 0;
		else if (animation = 3) animation = 1;
		frameAnimation = 0;
	}
	return false;
}
void Butterfly::render() const 
{
	texture->renderFrame(posAndDim.to_sdl(), animation, frameAnimation);
}
bool Butterfly::hit(const Rectangle& obj) {
	return posAndDim.hasIntersection(obj);
}
