#include "Bow.h"
#include "game.h"
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
bool Bow::update() 
{
    // MOVIMIENTO
    posAndDim.move(moveDir); 
    if (mouseMoved)
    {
        float bowCentre = getYPos();
        if ((moveDir.getY() > 0 && bowCentre > mouseY) || (moveDir.getY() < 0 && bowCentre < mouseY))
        {
            mouseMoved = false;
            moveDir = Vector2D<>(0, 0);
        }
    }
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
bool Bow::hit(const Rectangle& obj) 
{
    return false;
}
void Bow::handleEvent(const SDL_Event& event) 
{
    if (event.type == SDL_EVENT_MOUSE_MOTION)
    {
        mouseMoved = true;
        SDL_GetMouseState(nullptr, &mouseY);
        float bowCentre = getYPos();
        if (bowCentre < mouseY) moveDir = Vector2D<>(0, 2);
        else if (bowCentre > mouseY) moveDir = Vector2D<>(0, -2);
    }

    if (event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
    {
        // Anulamos el movimiento por el raton
        mouseMoved = false;
        if (event.key.key == SDLK_UP && posAndDim.getTopLeftCorner().getY() > 0.0f)
        {
            moveDir = Vector2D<>(0, -2);

        }
        else if (event.key.key == SDLK_DOWN && posAndDim.getTopLeftCorner().getY() < 500.0f)
        {
            moveDir = Vector2D<>(0, 2);

        }
        else if (event.key.key == SDLK_LEFT || event.button.button == SDL_BUTTON_LEFT)
        {
            if (!charged && numArrows > 0)
            {
                cargaArco = (float)SDL_GetTicks();
                charged = true;
                numArrows--;
            }
        }
        else if (event.key.key == SDLK_RIGHT || event.button.button == SDL_BUTTON_RIGHT)
        {
            if (charged)
            {
                charged = false;
                game->disparaFlecha();
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

float Bow::getChargeTime() const
{
    return cargaArco;
}
