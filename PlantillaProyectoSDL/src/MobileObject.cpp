#include "MobileObject.h"
#include "game.h"
using namespace std;

bool MobileObject::update(float dt) 
{
	posAndDim.move(speed);
	// Si se sale de la pantalla
	if (posAndDim.getTopLeftCorner().getX() > game->WINDOW_WIDTH || posAndDim.getTopLeftCorner().getY() + posAndDim.getHeight() < 0) return true;
	else return false;
}
