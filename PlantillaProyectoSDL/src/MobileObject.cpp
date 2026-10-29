#include "MobileObject.h"
using namespace std;
MobileObject::MobileObject() {

}
MobileObject::~MobileObject() {

}
void MobileObject::update() 
{
	posAndDim.move(speed);
}
