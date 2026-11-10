#include"Arrow.h"
#include"game.h"
using namespace std;

Arrow::Arrow(Rectangle posAndDim, Vector2D<> speed, Texture* texture, Game* game)
{
    this->texture = texture;
    this->posAndDim = posAndDim;
    this->speed = speed;
    this->game = game;
}

void Arrow::render()const
{
    texture->renderFrame(posAndDim.to_sdl(), 0, 0);
}

Rectangle Arrow::getPunta()const
{
    Point2D<> topLeft = Point2D<>(posAndDim.getTopLeftCorner().getX() + (3 * posAndDim.getWidth() / 4), posAndDim.getTopLeftCorner().getY());
    return Rectangle(topLeft, posAndDim.getWidth() / 4, posAndDim.getHeight());
}
int Arrow::GetCombo()const 
{
    return (globosExplotados - 1) * 5;
}
void Arrow::SumaCombo() {
    globosExplotados++;
}
