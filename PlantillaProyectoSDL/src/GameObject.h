#pragma once
//#include "game.h"
class Game;
class GameObject
{
private:
	
public:
	Game* game;
	virtual void render() const = 0;
	virtual	void update() = 0;

};
