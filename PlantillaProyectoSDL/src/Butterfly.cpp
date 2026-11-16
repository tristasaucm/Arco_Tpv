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
bool Butterfly::update(float dt){
	
	// Si se sale de la pantalla
	bool saleDrch = posAndDim.getTopLeftCorner().getX() > game->WINDOW_WIDTH - posAndDim.getWidth();
	bool saleIzq = posAndDim.getTopLeftCorner().getX() < 0;
	bool saleAbajo = posAndDim.getTopLeftCorner().getY() > game->WINDOW_HEIGHT - posAndDim.getHeight();
	bool saleArriba = posAndDim.getTopLeftCorner().getY() < 0;
	//Cambiamos la velocidad si se sale de la pantalla
	if (saleDrch)
	{
		speed = Point2D<>(-velRdB(gen1), velRdS(gen1));
	}if (saleIzq) {
		speed = Point2D<>(velRdB(gen1), velRdS(gen1));
	}
	if (saleArriba) {
		speed = Point2D<>(velRdS(gen1), velRdB(gen1));
	}if (saleAbajo) {
		speed = Point2D<>(velRdS(gen1), -velRdB(gen1));
	}
	
	posAndDim.move(speed);
	return false;

}
void Butterfly::render() const {
	//Falta añadir la animacion
	texture->renderFrame(posAndDim.to_sdl(), 0, 0);
}
bool Butterfly::hit(const Rectangle& obj) {
	return posAndDim.hasIntersection(obj);
}
