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
    if (charged) 
    {
        frame = 0;
        SDL_FRect indicador{
            indicador.x = posAndDim.getTopLeftCorner().getX() + posAndDim.getWidth(),
            indicador.y = posAndDim.getTopLeftCorner().getY() + posAndDim.getHeight()/2 - 5,
            indicador.w = 20,
            indicador.h = 20
        };
        if (getArrowSpeedM() >= 3) game->getTexture(game->BALLOON)->renderFrame(indicador, 1, 0);
        else if (getArrowSpeedM() >= 2) game->getTexture(game->BALLOON)->renderFrame(indicador, 2, 0);
        else game->getTexture(game->BALLOON)->renderFrame(indicador, 3, 0);
    }
    // Render
    texture->renderFrame(posAndDim.to_sdl(), 0, frame);
}
bool Bow::update(float dt) 
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

void Bow::handleEvent(const SDL_Event& event) 
{
    if (event.type == SDL_EVENT_MOUSE_MOTION)
    {
        mouseMoved = true;
        SDL_GetMouseState(nullptr, &mouseY);
        float bowCentre = getYPos();
        if (bowCentre < mouseY) moveDir = Vector2D<>(0, 3);
        else if (bowCentre > mouseY) moveDir = Vector2D<>(0, -3);
    }

    if (event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
    {
        // Anulamos el movimiento por el raton
        mouseMoved = false;
        if (event.key.key == SDLK_UP && posAndDim.getTopLeftCorner().getY() > 0.0f)
        {
            moveDir = Vector2D<>(0, -3);

        }
        else if (event.key.key == SDLK_DOWN && posAndDim.getTopLeftCorner().getY() < 500.0f)
        {
            moveDir = Vector2D<>(0, 3);

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

float Bow::getArrowSpeedM() const
{
    float ASpeedM = 1.0f + (SDL_GetTicks() - cargaArco) / 1000;
    if (ASpeedM > 3.0f)
    {
        ASpeedM = 3.0f;
    }
    return ASpeedM;
}
