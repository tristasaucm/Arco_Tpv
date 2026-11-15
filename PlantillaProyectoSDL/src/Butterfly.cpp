#include "Butterfly.h"
#include "game.h"
using namespace std;

random_device rd;
mt19937 gen(rd());
uniform_real_distribution<float> velRdB(2,4);

Butterfly::Butterfly(Rectangle posAndDim, Vector2D<> speed, Texture* texture, Game* game) 
{
	this->texture = texture;
	this->posAndDim = posAndDim;
	this->speed = speed;
	this->game = game;
}
bool Butterfly::update(float dt){
	
	// Si se sale de la pantalla
	bool saleDrch = posAndDim.getTopLeftCorner().getX() > game->WINDOW_WIDTH - posAndDim.getWidth();
	bool saleIzq = posAndDim.getTopLeftCorner().getX() < 0;
	bool saleAbajo = posAndDim.getTopLeftCorner().getY() > game->WINDOW_HEIGHT - posAndDim.getHeight();
	bool saleArriba = posAndDim.getTopLeftCorner().getY() < 0;
	//Cambiamos la velocidad si se sale de la pantalla
	if (saleDrch)
	{
		speed = Point2D<>(velRdB(gen), 0.0f);
	}if (saleIzq) {
		speed = Point2D<>(-velRdB(gen), 0.0f);
	}
	if (saleArriba) {
		speed = Point2D<>(0.0f,velRdB(gen));
	}if (saleAbajo) {
		speed = Point2D<>(0.0f, -velRdB(gen));
	}
	posAndDim.move(speed);
	return false;

}
