#pragma once
#include "MobileObject.h"
class Butterfly : public MobileObject
{
private:
	// Fila de animacion a renderizar
	int animation = 0;
	// Frame a renderizar de la animacion
	int frameAnimation = 0;
public:
	Butterfly(Rectangle, Vector2D<>, Texture*, Game*);
	bool update(float dt) override;
	void render() const override;
	bool hit(const Rectangle&) override;

};
