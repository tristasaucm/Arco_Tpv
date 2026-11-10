#pragma once
//#include "game.h"
class Game;
class GameObject
{
protected:
	Game* game;
	
public:
	virtual void render() const = 0;
	// Devuelve true si se sale de la pantalla
	virtual	bool update(float dt) = 0;
	virtual ~GameObject() {};
};
