#include "SceneObject.h"
using namespace std;

bool SceneObject::hit(const Rectangle& obj)
{
	return posAndDim.hasIntersection(obj);
}
