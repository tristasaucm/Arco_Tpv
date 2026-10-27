#include"InfoBar.h"
#include "texture.h"
#include "Bow.h"
#include "game.h"
using namespace std;

InfoBar::InfoBar(Game* game, Bow* bow, Texture* digits, Texture* letters, Texture* arrow2): game(game), bow(bow), digitsTexture(digits), lettersTexture(letters), arrow2Texture(arrow2) {}
InfoBar::~InfoBar(){}
// Renderiza un entero en la parte superior de la pantalla centrado en la xpos
void InfoBar::RenderNums(int num, float xpos)const
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
	// === Render de número de flechas === 
	RenderNums((float)bow->getNArrows(), posX);
	// --- Calculamos el numero de cifras del numero
	int cantidadCifras = 1;

	// - Cada vez que se puede dividir entre diez significa que tiene una cifra mas
	for (int aux = bow->getNArrows(); aux >= 10; aux = aux / 10) cantidadCifras++;
	//Poner correctamente con el ancho dependiendo del numero de cifras
	float anchoTotal = cantidadCifras * 29.0f;
	float xFlechaIndicadora = posX - anchoTotal - 70;

	// --- Flecha indicadora
	arrow2Texture->render(SDL_FRect(xFlechaIndicadora, 15.0f, 70.0f, 15.0f));
}
// Renderiza un indicador del nivel en el que estás jugando y la puntuación actual
void InfoBar::RenderPointsAndLevel(float xpos)const
{
	float LetterWidth = 50.0f;
	float LetterHeight = 30.0f;
	float Ypos = 10.0f;

	// --- Indicador del nivel
	lettersTexture->renderFrame(SDL_FRect(10.0f, Ypos, LetterWidth, LetterHeight), 0, 0);
	RenderNums(game->getLevel(), 80);

	// --- Calculamos el numero de cifras del numero
	int cantidadCifras = 1;

	// - Cada vez que se puede dividir entre diez significa que tiene una cifra mas
	for (int aux = game->getPoints(); aux >= 10; aux = aux / 10) cantidadCifras++;

	// --- Calculamos la posicion de la primera cifra para que el numero siempre este centrado
	float x = xpos - (cantidadCifras * 29.0f) / 2 - 5 * 60.0f;

	// === S C O R E ===
	lettersTexture->renderFrame(SDL_FRect(x, Ypos, LetterWidth, LetterHeight), 3, game->getFrameLetras());
	lettersTexture->renderFrame(SDL_FRect(x + 60, Ypos, LetterWidth, LetterHeight), 2, game->getFrameLetras());
	lettersTexture->renderFrame(SDL_FRect(x + 120, Ypos, LetterWidth, LetterHeight), 6, game->getFrameLetras());
	lettersTexture->renderFrame(SDL_FRect(x + 180, Ypos, LetterWidth, LetterHeight), 4, game->getFrameLetras());
	lettersTexture->renderFrame(SDL_FRect(x + 240, Ypos, LetterWidth, LetterHeight), 1, game->getFrameLetras());

	RenderNums((float)game->getPoints(), xpos);
}
void InfoBar::render() const 
{
	// === Render de puntuación ===
	RenderPointsAndLevel(game->WINDOW_WIDTH/2 + 100.0f);
	// === Render de Numero de Flechas ===
	RenderNumFlechas(game->WINDOW_WIDTH - 30.0f);
}
