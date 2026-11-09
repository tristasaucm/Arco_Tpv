#pragma once
#include "MobileObject.h"
class Arrow : public MobileObject
{
private:
	
public:
	Arrow(Rectangle, Vector2D<>, Texture*, Game*);
	~Arrow();
	void render() const override;
	bool hit(const Rectangle&) override;
};
