#pragma once
#include "rectangle.h"
#include "vector2D.h"
#include "texture.h"
#include "Bow.h"
class Game;

class InfoBar {
private:
	Game* game;
	Bow* bow;
	Texture* digitsTexture;
	Texture* lettersTexture;
	Texture* arrow2Texture;
public:
	InfoBar(Game*, Bow*, Texture*, Texture*, Texture*);
	~InfoBar();
	void RenderNums(int, float)const;
	void RenderNumFlechas(float )const;
	void RenderPointsAndLevel(float)const;
	void render() const;
};
