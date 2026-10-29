#include "Bow.h"
using namespace std;

Bow::Bow(Rectangle posDim, int numArrows, Texture* texture, Game* game)
{
    this->posAndDim = posDim; 
    this->numArrows = numArrows; 
    this->texture = texture;
    this->game = game;
}
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
void Bow::update() 
{
    // MOVIMIENTO
    posAndDim.move(moveDir);
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
}

void Bow::handleEvent(const SDL_Event& event) 
{
    if (event.type == SDL_EVENT_KEY_DOWN) 
    {
        // Movimiento hacia arriba
        if (event.key.key == SDLK_UP && posAndDim.getTopLeftCorner().getY() > 0.0f)
        {
            moveDir = Vector2D<>(0, -0.25f);

        }
        // Movimiento hacia abajo
        else if (event.key.key == SDLK_DOWN && posAndDim.getTopLeftCorner().getY() < 500.0f)
        {
            moveDir = Vector2D<>(0, 0.25f);

        }
        // Cargar arco
        else if (event.key.key == SDLK_LEFT && !charged && numArrows > 0)
        {
            charged = true;
            numArrows--;
        }
        // Disparar
        else if (event.key.key == SDLK_RIGHT && charged)
        {
            this->game->disparaFlecha();
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
