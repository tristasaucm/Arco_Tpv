#include "game.h"
#include <vector>
#include <string>

#include <SDL3_image/SDL_image.h>

#include "texture.h"
#include <iostream>
#include <ctime>
#include <fstream>

using namespace std;

// --- Generacion aleatorios
random_device rd;
mt19937 gen(rd());

// Aqui se encuentran los valores entre los que se varia para el random de los globos
uniform_int_distribution<int> colorRd(0, 6);
uniform_int_distribution<int> PosRd(200, 700);
uniform_real_distribution<float> velRd(-0.5f, -0.1f);


using uint = unsigned int; // alias para el tipo unsigned int
constexpr uint WIN_WIDTH = 800;
constexpr uint WIN_HEIGHT = 600;


// Constantes
constexpr const char* const WINDOW_TITLE = "Bow & Arrow 1.0";
constexpr const char* IMAGE_ROOT = "../assets/images/";
constexpr const char* LEVEL_ROOT = "../assets/levels/";

// Estructura para especificar las texturas que hay que
// cargar y el tamaño de su matriz de frames
struct TextureSpec
{
	const char* name;
	int nrows = 1;
	int ncols = 1;
};

constexpr array<TextureSpec, Game::NUM_TEXTURES> textureList{
	TextureSpec{"bow.png", 1, 2},
	{"arrow.png"},
	{"balloons.png", 7, 6},
	{"arrow2.png"},
	{"digits.png", 1, 10},
	{"rewards.png", 10, 8},
};
void Game::leeArchivoNivel(int level)
{
	std::ifstream archivo(LEVEL_ROOT+ to_string(level) + ".txt"); // Abre el archivo

	std::string clave;
	int Numarrows = 0;
	// Lee el archivo ignorando la primera línea de texto y guardando los datos numéricos
	while (archivo >> clave) {
		if (clave == "bgcolor") {
			archivo >> bg_r >> bg_g >> bg_b;
		}
		else if (clave == "arrows") {
			archivo >> Numarrows;
		}
		else if (clave == "baldelay") {
			archivo >> baldelay;
		}
		else if (clave == "balpoints") {
			archivo >> balpoints;
		}
		else if (clave == "goal") {
			archivo >> goal;
		}
	}
	archivo.close();// cerramos el archivo
	bow->setNumArrows(Numarrows);//Cambiamos el numero de flechas
}
 void Game::BalloonGeneration() {

	 // Genera 10 globos a la vez cuando ya no queda ninguno
	if (todosExplotados)
	{
		if (balloons.size() < 10 && SDL_GetTicks() - creacionGlobo > baldelay) {
			int col = colorRd(gen);
			float pos = (float)PosRd(gen);

			Rectangle posDim = Rectangle(Point2D<>(pos, 700.0f), 65.0f, 75.0f);
			Vector2D<> speed = Point2D<>(0.0f, velRd(gen));
			Balloon* bal = new Balloon(posDim, speed, col, textures[BALLOON]);

			balloons.push_back(bal);
			//guardamos el tiempo de creaccion del ultimo globo
			numGlobosGenerados++;
			creacionGlobo = (int)SDL_GetTicks();
		}

		if (numGlobosGenerados >= 10) {
			todosExplotados = false;
		}
		
	}
	else {
		if (balloons.size() == 0) {
			todosExplotados = true;
			numGlobosGenerados = 0;
		}
	}
 }
Game::Game()
{
	exit = false;
	// Carga SDL y crea la ventana con su renderizador
	if (!SDL_Init(SDL_INIT_VIDEO))
		throw "SDL_Init: "s + SDL_GetError();

	if (!SDL_CreateWindowAndRenderer(WINDOW_TITLE,
	                                 WINDOW_WIDTH, WINDOW_HEIGHT,
	                                 0, &window, &renderer))
		throw "window or renderer: "s + SDL_GetError();
	// Carga las texturas al inicio
	try {
		textures.fill(nullptr);
		string imgRoot = IMAGE_ROOT;

		for (size_t i = 0; i < textures.size(); i++) {
			auto [name, nrows, ncols] = textureList[i];
			textures[i] = new Texture(renderer, (imgRoot + name).c_str(), nrows, ncols);
		}
	}
	catch (...) {
		// Liberar la memoria reservada
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		for (size_t i = 0; i < textures.size(); i++) {
			delete textures[i];
		}

		throw; // relanza la excepción
	}
	// Configura que se pueden utilizar capas translúcidas
	// SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	bow = new Bow(Rectangle(Point2D<>(0.0f, 250.0f), 70, 100), 10, textures[BOW],this);//Creamos el arco
	//Limpiamos los vectores que vamos a usar
	arrows.clear();
	balloons.clear();
	//Establecemos el primer nivel que vamos a elegir y lo cargamos
	level = 1;
	leeArchivoNivel(level);
	delta = (int)SDL_GetTicks();
	hud = new InfoBar(this, bow);
}

Game::~Game()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	
	delete bow;// Borramos el arco
	delete hud; // Borramos el HUD

	// Liberar todas las flechas restantes en el vector
	for (Arrow* a : arrows) {
		delete a;
	}
	arrows.clear();
	// Liberar todos los globos restantes en el vector
	for (Balloon* b : balloons) {
		delete b;
	}
	balloons.clear();

	//Liberamos la memoria que ocupan las texturas
	for (size_t i = 0; i < textures.size(); i++) {
		delete textures[i];
	}
	
}
void Game::disparaFlecha() 
{
	// Creamos una flecha, la disparamos y la metemos en el vector de flechas
	float arrowY = bow->getYPos() - 6.0f;

	// Multiplicador de velocidad
	float hSpeedM = 1.0f + (SDL_GetTicks() - cargaArco) / 1000;
	if (hSpeedM > 3.0f) hSpeedM = 3.0f;

	Arrow* newArrow = new Arrow(Rectangle(Point2D<>(0.0f, arrowY), 70, 21), Vector2D<>(0.25f * hSpeedM, 0.0f), textures[ARROW], this);
	arrows.push_back(newArrow);
}
void Game::render() const
{
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, bg_r, bg_g, bg_b, 255);

	bow->render();

	// Renderizar todas las flechas del vector
	for (const Arrow* a : arrows) {
		if (a != nullptr) {
			a->render();
		}
	}
	
	// Renderizar todos los globos del vector
	for (const Balloon* b : balloons) {
		if (b != nullptr) {
			b->render();
		}
	}

	// Render del hud
	hud->render();

	SDL_RenderPresent(renderer);
}
void Game::UpdateBallons() {

	for (auto it = balloons.begin(); it != balloons.end(); )
	{
		(*it)->update();
		++it;
	}
}
void Game::UpdateArrows() {
	for (auto it = arrows.begin(); it != arrows.end(); )
	{
		(*it)->update();
		++it;
	}
}
void Game::update()
{
	//actualizamos el arco
	bow->update();

	// Actualizar y limpiar flechas que salen de la pantalla
	UpdateArrows();

	// Actualizar y destruir los globos explotados
	UpdateBallons();

	// Genera globos con intervalo de tiempo
	BalloonGeneration();

}

void
Game::run()
{
	while (!exit) 
	{

		// delta = Tiempo de ahora - Tiempo frame anterior
		delta = (int)SDL_GetTicks() - delta;
		handleEvents();
		update();
		render();
		compruebaDerrota();
		compruebaPasoNivel();
		// Esperar = Tiempo que debe pasar - Tiempo que ha pasado
		SDL_Delay(FRAME_PERIOD - delta);
		// delta = tiempo de ahora
		delta = (int)SDL_GetTicks();
	}
}

void
Game::handleEvents()
{
	SDL_Event event;
	// Only quit is handled directly, everything else is delegated
	if (SDL_PollEvent(&event)) {
		if (event.type == SDL_EVENT_QUIT || (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE))
			exit = true;
		// Si has perdiddo y pulsas la R para reiniciar el nivel
		if (pierde && event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_R)
		{
			// Eliminamos los globos que queden
			for (Balloon* b : balloons)
			{
				delete b;
			}
			balloons.clear();
			// Reiniciamos la puntuacion
			pierde = false;
			pasaNivel = false;
			points = 0;
			goal = -1;
			level = 1;
			// Volvemos a cargar el nivel
			leeArchivoNivel(level);
		}
		else 
		{
			// Para Cambiar la velocidad de la flecha en funcion de cuanto tiempo esta cargada
			if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_LEFT && !bow->isCharged()) cargaArco = (int)SDL_GetTicks();
			
			// Manejo de eventos del arco
			bow->handleEvent(event);
		}
	}
}

bool Game::checkCollision(const Rectangle& rect)
{
	bool golpea = false;
	for (auto it = balloons.begin(); it != balloons.end(); )
	{
		golpea = (*it)->hit(rect);
		if (golpea) aumentaPuntuacion();
		++it;
	}
	return golpea;
}

void Game::aumentaPuntuacion() 
{
	points += balpoints;
}

void Game::compruebaDerrota() 
{
	if (bow->getNArrows() == 0 && !bow->isCharged() && arrows.size() == 0 && ((points < goal && goal > 0) || goal < 0))
	{
		pierde = true;
	}
}

void Game::compruebaPasoNivel()
{
	// Pasamos al siguiente nivel
	if (pasaNivel) 
	{
		pierde = false;
		pasaNivel = false;
		points = 0;
		goal = -1;
		level++;
		leeArchivoNivel(level);
	}
	// Si hay una meta y la has superado indicamos que hay que pasar al siguiente nivel
	else if (goal > 0 && points >= goal)
	{
		pasaNivel = true;
	}
}

int Game::getLevel() const
{
	return level;
}
int Game::getPoints() const
{
	return points;
}
bool Game::derrota() const
{
	return pierde;
}
bool Game::nivelCompletado() const
{
	return pasaNivel;
}
