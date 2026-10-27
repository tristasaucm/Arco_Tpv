#include "rectangle.h"
#include "vector2D.h"
#include "game.h"
#include "texture.h"


Bow::Bow(Rectangle posDim, int numarrows, Texture* texture): posAndDim(posDim), numArrows(numarrows), texture(texture) {}
Bow::~Bow() {}
void Bow::render() const
{
    // Frame arco no cargado
    int frame = 1;
    // Frame arco cargados
    if (charged) frame = 0;
    // Render
    texture->renderFrame(posAndDim.to_sdl(), 0, frame);
}
bool Bow::update(float delta) 
{
    // MOVIMIENTO
    posAndDim.move(moveDir * delta);
    // ---Limite de movimiento hacia arriba
    if (posAndDim.getTopLeftCorner().getY() < 0.0f)
    {
        Vector2D<> ajuste(0, 0.0f - posAndDim.getTopLeftCorner().getY());
        posAndDim.move(ajuste);
    }
    // ---Limite de movimiento hacia abajo
    else if (posAndDim.getTopLeftCorner().getY() > 500.0f)
    {
        Vector2D<> ajuste(0, 500.0f - posAndDim.getTopLeftCorner().getY());
        posAndDim.move(ajuste);
    }


    return false;
}

void Bow::handleEvent(const SDL_Event& event) {
    if (event.type == SDL_EVENT_KEY_DOWN) {

        if (event.key.key == SDLK_UP && posAndDim.getTopLeftCorner().getY() > 0.0f)
        {
            moveDir = Vector2D<>(0, -0.25f);

        }
        else if (event.key.key == SDLK_DOWN && posAndDim.getTopLeftCorner().getY() < 500.0f)
        {
            moveDir = Vector2D<>(0, 0.25f);

        }
        else if (event.key.key == SDLK_LEFT)
        {
            if (!charged && numArrows > 0)
            {
                charged = true;
                numArrows--;
            }
        }
    }

    else if (event.type == SDL_EVENT_KEY_UP)
    {
        if (event.key.key == SDLK_UP || event.key.key == SDLK_DOWN)
        {
            moveDir = Vector2D<>(0, 0);
        }
    }
}
float Bow::getYPos() const 
{
    return posAndDim.getTopLeftCorner().getY() + posAndDim.getHeight() / 2;
}

bool Bow::Shoot()
{
    if (charged)
    {
        charged = false;
        return true;
    }
    else return false;
}

int Bow::getNArrows() const
{
    return numArrows;
}

void Bow::setNumArrows(int numArrows) 
{
    this->numArrows = numArrows;
}

bool Bow::isCharged() const
{
    return charged;
}
