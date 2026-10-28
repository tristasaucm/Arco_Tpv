#include"Arrow.h"
using namespace std;

Arrow::Arrow(Rectangle posAndDim, Vector2D<> speed, Texture* texture, Game* game)
{
    this->texture = texture;
    this->posAndDim = posAndDim;
    this->speed = speed;
    this->game = game;
}
Arrow::~Arrow() {}
void Arrow::render() const
{
    texture->renderFrame(posAndDim.to_sdl(), 0, 0);
}


