#include"InfoBar.h"
#include "texture.h"
#include "Bow.h"
#include "game.h"
using namespace std;

InfoBar::InfoBar(Game* game, Bow* bow)
{
	this->game = game;
	this->bow = bow;
	digitsTexture = game->getTexture(game->DIGITS);
	lettersTexture = game->getTexture(game->REWARDS);
	arrow2Texture = game->getTexture(game->ARROWSLEFT);
	balloon = game->getTexture(game->BALLOON);
}
InfoBar::~InfoBar(){}
// Renderiza un entero en la parte superior de la pantalla centrado en la xpos
void InfoBar::RenderEnteros(int num, float xpos)const
{
	// === Render de puntuación ===

	// --- Calculamos el numero de cifras del numero
	int cantidadCifras = 1;

	// - Cada vez que se puede dividir entre diez significa que tiene una cifra mas
	for (int aux = num; aux >= 10; aux = aux / 10) cantidadCifras++;

	// --- Calculamos la posicion de la primera cifra para que el numero siempre este centrado
	float anchoTotal = cantidadCifras * 29.0f;
	float xPrimeraCifra = xpos - anchoTotal / 2.0f - 27.5f;

	int puntuacionAux = num;

	do // --- Colocamos todas las cifras empezando por la última usando do while para que minimo se pinte el primer 0
	{
		int cifra = puntuacionAux % 10;
		digitsTexture->renderFrame(SDL_FRect(xPrimeraCifra + cantidadCifras * 29.0f, 10.0f, 27.5f, 30.0f), 0, cifra);

		puntuacionAux = puntuacionAux / 10;
		cantidadCifras--;

	} while (cantidadCifras > 0);
}
// Renderiza el numero de flechas restantes
void InfoBar::RenderNumFlechas(float posX)const 
{
	float width = 45.0f;
	float height = 8.0f;
	float posPrimeraFlechaX = posX - (bow->getNArrows() * (height + 10));
	double angle = 270.0;
	for (int i = 0; i < bow->getNArrows(); i++)
	{
		SDL_FRect target = SDL_FRect(posPrimeraFlechaX + i * (height + 10), width - 15, width, height);
		arrow2Texture->renderFrame(target, 0, 0, angle, nullptr, SDL_FLIP_NONE);
	}
}
// Renderiza un indicador del nivel en el que estás jugando y la puntuación actual
void InfoBar::RenderPointsAndLevel(float xpos)const
{
	// --- Indicador del nivel
	lettersTexture->renderFrame(SDL_FRect(10.0f, 10, 50.0f, 30.0f), 0, 0);
	RenderEnteros(game->getLevel(), 80);
	
	// --- Globo indicador de puntos explotando
	
	// Calculamos el numero de cifras del numero
	int cantidadCifras = 1;
	for (int aux = game->getPoints(); aux >= 10; aux = aux / 10) cantidadCifras++;

	// Calculamos la posicion del globo explotando
	float x = xpos - (cantidadCifras * 29.0f) / 2 - 50.0f;

	balloon->renderFrame(SDL_FRect(x, 0, 50.0f, 60.0f), 1, frameLetras);

	// --- Puntos
	RenderEnteros(game->getPoints(), xpos);
}
void InfoBar::RenderDerrota()const
{
	float posX = game->WINDOW_WIDTH / 2;
	float posY = game->WINDOW_HEIGHT / 2 - 30;

	// === P E R D E R ===
	lettersTexture->renderFrame(SDL_FRect(posX - 295, posY, 90, 70), 9, frameLetras);
	lettersTexture->renderFrame(SDL_FRect(posX - 195, posY, 90, 70), 1, frameLetras);
	lettersTexture->renderFrame(SDL_FRect(posX - 95, posY, 90, 70), 4, frameLetras);
	lettersTexture->renderFrame(SDL_FRect(posX + 5, posY, 90, 70), 6, frameLetras);
	lettersTexture->renderFrame(SDL_FRect(posX + 105, posY, 90, 70), 1, frameLetras);
	lettersTexture->renderFrame(SDL_FRect(posX + 205, posY, 90, 70), 4, frameLetras);

	// === R Flecha RESET ===
	posY = game->WINDOW_HEIGHT;
	lettersTexture->renderFrame(SDL_FRect(posX - 170, posY, 45, 35), 4, frameLetras);

	arrow2Texture->renderFrame(SDL_FRect(posX - 125, posY + 10, 70, 21), 0, 0);

	lettersTexture->renderFrame(SDL_FRect(posX - 50, posY, 45, 35), 4, frameLetras);
	lettersTexture->renderFrame(SDL_FRect(posX, posY, 45, 35), 1, frameLetras);
	lettersTexture->renderFrame(SDL_FRect(posX + 50, posY, 45, 35), 3, frameLetras);
	lettersTexture->renderFrame(SDL_FRect(posX + 100, posY, 45, 35), 1, frameLetras);
	lettersTexture->renderFrame(SDL_FRect(posX + 150, posY, 45, 35), 8, frameLetras);
}
void InfoBar::RenderPasoNivel()const
{
	// Render normal de fondo
	render();

	float posX = game->WINDOW_WIDTH / 2;
	float posY = game->WINDOW_HEIGHT / 2 - 30;

	// === S E T   L E B E L === (No hay V)
	lettersTexture->renderFrame(SDL_FRect(posX - 345, posY, 72, 56), 3, frameLetras);
	lettersTexture->renderFrame(SDL_FRect(posX - 265, posY, 72, 56), 1, frameLetras);
	lettersTexture->renderFrame(SDL_FRect(posX - 185, posY, 72, 56), 8, frameLetras);
	lettersTexture->renderFrame(SDL_FRect(posX - 40, posY, 72, 56), 0, frameLetras);
	lettersTexture->renderFrame(SDL_FRect(posX + 40, posY, 72, 56), 1, frameLetras);
	lettersTexture->renderFrame(SDL_FRect(posX + 120, posY, 72, 56), 7, frameLetras);
	lettersTexture->renderFrame(SDL_FRect(posX + 200, posY, 72, 56), 1, frameLetras);
	lettersTexture->renderFrame(SDL_FRect(posX + 280, posY, 72, 56), 0, frameLetras);
}
void InfoBar::render() const 
{
	// --- Render de puntuación
	RenderPointsAndLevel(game->WINDOW_WIDTH/2);
	// --- Render de Numero de Flechas
	RenderNumFlechas(game->WINDOW_WIDTH - 30.0f);
	// --- Render derrota
	if (game->derrota()) RenderDerrota();
	// --- Render paso de nivel
	if (game->nivelCompletado()) RenderPasoNivel();
}

void InfoBar::update() 
{
	// Actualizamos el frame de la animacion del globo indicador de puntos explotando
	frameLetras++;
	// Volver al primer frame de la animación
	if (frameLetras > balloon->getNumColumns()) frameLetras = 0;
}
