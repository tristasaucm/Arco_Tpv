#pragma once
#include "MobileObject.h"
class Arrow : public MobileObject
{
private:
	int globosExplotados = 0;
public:
	Arrow(Rectangle, Vector2D<>, Texture*, Game*);
	
	void render() const override;
	Rectangle getPunta() const;
	int GetCombo() const;
	void SumaCombo();
};
