#pragma once
#include "Bow.h"
#include "GameObject.h"
class Game;

class InfoBar : public GameObject
{
private:
	Bow* bow;
	Texture* digitsTexture;
	Texture* lettersTexture;
	Texture* arrow2Texture;
	Texture* balloon;
	// Frame a renderizar de las letras
	int frameLetras = 0;
public:
	InfoBar(Game*, Bow*);
	~InfoBar();
	void RenderEnteros(int, float)const;
	void RenderNumFlechas(float )const;
	void RenderPointsAndLevel(float)const;
	void RenderDerrota() const;
	void RenderPasoNivel() const;

	void render() const override;
	void update() override;
};
