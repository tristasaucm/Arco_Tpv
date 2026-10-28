#include "MobileObject.h"
using namespace std;

void MobileObject::update() 
{
	posAndDim.move(speed);
}
