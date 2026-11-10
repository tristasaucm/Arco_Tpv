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
uniform_real_distribution<float> velRd(-4, -2);


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
	{"butterfly.png", 4, 10},
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

	// TODO: Cambiamos el numero de flechas
	// bow->setNumArrows(Numarrows);
}
 void Game::BalloonGeneration() 
 {
	// Genera globos seguidos con delay cuando haya menos de 10
	if (numGlobosGenerados < 10 && SDL_GetTicks() - creacionGlobo > baldelay)
	{
		int col = colorRd(gen);
		float pos = (float)PosRd(gen);

		Rectangle posDim = Rectangle(Point2D<>(pos, 700.0f), 65.0f, 70.0f);
		Vector2D<> speed = Point2D<>(0.0f, velRd(gen));
		Balloon* bal = new Balloon(posDim, speed, col, textures[BALLOON]);

		objetos.push_back(bal);
		creacionGlobo = (int)SDL_GetTicks();
		//guardamos el tiempo de creaccion del ultimo globo
		numGlobosGenerados++;
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
	//Establecemos el primer nivel que vamos a elegir y lo cargamos
	level = 1;
	leeArchivoNivel(level);
	delta = (int)SDL_GetTicks();

	Hud = new InfoBar(this, bow);
}

Game::~Game()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	
	//TODO:Liberar la lista
	
	delete bow;// Borramos el arco
	delete Hud; // Borramos el HUD
	for (SceneObject* obj : objetos) {
		delete obj;
	}
	//Liberamos la memoria que ocupan las texturas
	for (size_t i = 0; i < textures.size(); i++) {
		delete textures[i];
	}
	
}
void Game::disparaFlecha() 
{
	// TODO:Meter Flecha a la lista
	
	// Creamos una flecha, la disparamos y la metemos en el vector de flechas
	float arrowY = bow->getYPos() - 6.0f;

	// Multiplicador de velocidad
	float hSpeedM = bow->getArrowSpeedM();

	Arrow* newArrow = new Arrow(Rectangle(Point2D<>(0.0f, arrowY), 70, 21), Vector2D<>(2 * hSpeedM, 0.0f), textures[ARROW], this);
	objetos.push_back(newArrow);
	numFlechasActivas++;
}
void Game::render() const
{
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, bg_r, bg_g, bg_b, 255);

	bow->render();
	//TODO: Render de toda la lista
	for (auto it = objetos.begin(); it != objetos.end(); ++it) 
	{
		(*it)->render();
	}
	Hud->render();
	SDL_RenderPresent(renderer);
}
void Game::update(float dt)
{
	bow->update(dt);
	//TODO: Actualizar la lista
	for (auto it = objetos.begin(); it != objetos.end();)
	{
		if ((*it)->update(dt))
		{
			// Si es un globo
			if (dynamic_cast<Balloon*>(*it))
			{
				// Indicamos que se ha reducido el numero de globos para que se genere otro
				numGlobosGenerados--;
			}
			// Si es una flecha
			else 
			{
				numFlechasActivas--;
			}
			delete* it;
			it = objetos.erase(it);
		}
		else
		{
			++it;
		}
	}
	Hud->update(dt);
	// Genera globos con intervalo de tiempo
	BalloonGeneration();
	if (numFlechasActivas > 0) 
	{
		for (auto obj = objetos.begin(); obj != objetos.end();++obj)
		{
			Arrow* arrow = dynamic_cast<Arrow*>(*obj);

			if (arrow != nullptr)
			{
				checkCollision((*arrow).getPunta());
			}
		}
	}
}

void
Game::run()
{
	while (!exit) 
	{

		// delta = Tiempo de ahora - Tiempo frame anterior
		delta = (int)SDL_GetTicks() - delta;
		handleEvents();
		update(delta);
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
			//TODO: Vaciar la lista
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
			// Manejo de eventos del arco
			bow->handleEvent(event);
		}
	}
}

bool Game::checkCollision(const Rectangle& rect)
{
	for (auto obj = objetos.begin(); obj != objetos.end();)
	{
		if ((*obj)->hit(rect)) 
		{
			numGlobosGenerados--;
			aumentaPuntuacion();
			delete* obj;
			obj = objetos.erase(obj);
		}
		else 
		{
			++obj;
		}
	}
	return false;
}

void Game::aumentaPuntuacion() 
{
	points += balpoints;
}

void Game::compruebaDerrota() 
{
	// TODO
	
	if (bow->getNArrows() == 0 && !bow->isCharged() && numFlechasActivas == 0 && ((points < goal && goal > 0) || goal < 0))
	{
		pierde = true;
	}
	
}

void Game::compruebaPasoNivel()
{
	// Pasamos al siguiente nivel (Despues de 2 segundos para indicar el paso)
	if (pasaNivel && SDL_GetTicks() > tiempoPasoNivel + 2000) 
	{
		pierde = false;
		pasaNivel = false;
		points = 0;
		goal = -1;
		level++;
		leeArchivoNivel(level);
	}
	// Si hay una meta y la has superado indicamos que hay que pasar al siguiente nivel
	else if (!pasaNivel && goal > 0 && points >= goal)
	{
		pasaNivel = true;
		tiempoPasoNivel = (float)SDL_GetTicks();
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
