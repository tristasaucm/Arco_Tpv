#ifndef GAME_H
#define GAME_H

#include <vector>
#include <SDL3/SDL.h>
#include <array>
#include <istream>
#include <random>
#include <string>
#include "vector2D.h"
#include "rectangle.h"
#include "GameObject.h"
#include "SceneObject.h"
#include "MobileObject.h"
#include "Balloon.h"
#include "Arrow.h"
#include "Bow.h"
#include "InfoBar.h"

// Declaraciones anticipadas
class Texture;

/**
 * Clase principal del juego.
 */
class Game
{
public:

	// Número de actualizaciones por segundo
	static constexpr int FRAME_RATE = 30;
	// Periodo entre actualizaciones en milisegundos
	static constexpr int FRAME_PERIOD = 1000 / FRAME_RATE;
	// Tamaño real de la ventana
	static constexpr int WINDOW_WIDTH = 800;
	static constexpr int WINDOW_HEIGHT = 600;
	enum TextureName
	{
		BOW = 0,
		ARROW,
		BALLOON,
		ARROWSLEFT,
		DIGITS,
		REWARDS,
		NUM_TEXTURES
	};

private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	std::array<Texture*, NUM_TEXTURES> textures;
	
	// Lee el nivel desde archivo
	void leeArchivoNivel(int);
	// Genera un globo si ha pasado el tiempo suficiente desde el anterior
	void BalloonGeneration();
	// Aumenta la puntuacion la cantidad de balpoints
	void aumentaPuntuacion();
	// Devuelve true cuando te has quedado sin flechas y no has alcanzado la meta de puntos
	void compruebaDerrota();
	// Comprueba si has completado un nivel y pasa al siguiente nivel 
	void compruebaPasoNivel();

	// Render
	void render() const;

	// --- Update

	void UpdateBallons();
	void UpdateArrows();
	void update();

	// --- Handle events

	void handleEvents();

	bool exit;

	// --- Elementos del juego
	// HUD del juego con nivel, puntuacion y flechas restantes
	InfoBar* hud;
	// Arco
	Bow* bow;
	// Array de flechas
	std::vector<Arrow*> arrows;
	// Array de globos
	std::vector<Balloon*> balloons;

	// --- Variables de tiempo y frames
	// Tiempo entre frames del update
	int delta;
	// Momento en el que se ha creado el globo anterior
	int creacionGlobo = 0;
	// Momento en el que se ha cargado la flecha
	int cargaArco = 0;

	// --- Configuración del nivels
	// Puntuación del nivel (Se reinicia al cambiar de nivel)
	int points = 0;
	// Valores red, green y blue del background
	int bg_r = 0, bg_g = 0, bg_b = 0;
	// Tiempo en ms entre la generación de un globo y el otro
	int baldelay = 0;
	// Puntos que suma cada globo
	int balpoints = 0;
	// Puntos objetivo del nivel
	int goal = 0;
	// Nivel actual
	int level = 0;
	// Comprueba si se han explotado todos los globos
	bool todosExplotados = true;
	//Numero de globos que se han generado
	int numGlobosGenerados = 0;

	// Indica si has perdido
	bool pierde = false;
	// Indica si has pasado de nivel
	bool pasaNivel = false;

public:
	Game();
	~Game();

	// Obtiene una textura por su nombre
	Texture* getTexture(TextureName name) const;

	// Ejecuta el bucle principal del juego
	void run();

	// Crea una flecha y la mete al vector de flechas
	void disparaFlecha();
	// Comprueba si hay algún objeto colocado en ese rectángulo
	bool checkCollision(const Rectangle& rect);

	// Devuelve el nivel actual
	int getLevel() const;
	// Devuelve la puntuación actual
	int getPoints() const;
	// Devuelve si has perdido
	bool derrota() const;
	// Devuelve si has pasado de nivel
	bool nivelCompletado() const;
};

inline Texture*
Game::getTexture(TextureName name) const
{
	return textures[name];
}

#endif // GAME_H
