#pragma once
#include "SceneObject.h"
class MobileObject : public SceneObject
{
private:
	
public:
	Vector2D<> speed;
	MobileObject();
	~MobileObject();
	void update() override;
	virtual void render() const = 0;


};
