#include"game.h"
#include "rectangle.h"
#include "vector2D.h"
#include "texture.h"
using namespace std;

Arrow::Arrow(Rectangle posAndDim, Vector2D<> speed, Texture* texture, Game* game): texture(texture)
{
    this->posAndDim = posAndDim;
    this->speed = speed;
    this->game = game;
}
Arrow::~Arrow() {}
void Arrow::render() const
{
    texture->renderFrame(posAndDim.to_sdl(), 0, 0);
}

bool Arrow::update(float delta)
{
    bool ret = false;
    // Si ha sido disparada
       // Movimiento
        posAndDim.move(speed *delta);

        // Colisiones
        Rectangle Punta = Rectangle(Point2D<>(posAndDim.getTopLeftCorner().getX() + (3*posAndDim.getWidth()/4), posAndDim.getTopLeftCorner().getY()), posAndDim.getWidth() / 4, posAndDim.getHeight());
        game->checkCollision(Punta);

        // Si se ha salido de la pantalla devuelve true para que el game la destruya
        if (posAndDim.getTopLeftCorner().getX() > game->WINDOW_WIDTH + 10) ret = true;
    return ret;
}

