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
Arrow::~Arrow() {}
void Arrow::render() const
{
    /*
        Colisiones
        Point2D<> topLeft = Point2D<>(posAndDim.getTopLeftCorner().getX() + (3 * posAndDim.getWidth() / 4), posAndDim.getTopLeftCorner().getY());
        Rectangle punta = Rectangle(topLeft, posAndDim.getWidth() / 4, posAndDim.getHeight());
        game->checkCollision(punta);
    */
    texture->renderFrame(posAndDim.to_sdl(), 0, 0);
}


