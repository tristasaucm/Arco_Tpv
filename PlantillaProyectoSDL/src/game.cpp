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
			creacionGlobo = (float)SDL_GetTicks();
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
	bow = new Bow(Rectangle(Point2D<>(0.0f, 250.0f), 70, 100), 10, textures[BOW]);//Creamos el arco
	//Limpiamos los vectores que vamos a usar
	arrows.clear();
	balloons.clear();
	//Establecemos el primer nivel que vamos a elegir y lo cargamos
	level = 1;
	leeArchivoNivel(level);
	delta = (float)SDL_GetTicks();
	hud = new InfoBar(this, bow, getTexture(DIGITS), getTexture(REWARDS), getTexture(ARROWSLEFT));
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
	newArrow->dispara();
	arrows.push_back(newArrow);
}
void Game::RenderDerrota()const 
{
	float posX = WINDOW_WIDTH / 2;
	float posY = WINDOW_HEIGHT / 2 - 30;
	
	// === P E R D E R ===
	textures[REWARDS]->renderFrame(SDL_FRect(posX - 295, posY, 90, 70), 9, frameLetras);
	textures[REWARDS]->renderFrame(SDL_FRect(posX - 195, posY, 90, 70), 1, frameLetras);
	textures[REWARDS]->renderFrame(SDL_FRect(posX - 95, posY, 90, 70), 4, frameLetras);
	textures[REWARDS]->renderFrame(SDL_FRect(posX + 5, posY, 90, 70), 6, frameLetras);
	textures[REWARDS]->renderFrame(SDL_FRect(posX + 105, posY, 90, 70), 1, frameLetras);
	textures[REWARDS]->renderFrame(SDL_FRect(posX + 205, posY, 90, 70), 4, frameLetras);

	// === R Flecha RESET ===

	textures[REWARDS]->renderFrame(SDL_FRect(posX - 170, WINDOW_HEIGHT - 70, 45, 35), 4, frameLetras);

	textures[ARROW]->renderFrame(SDL_FRect(posX - 125, WINDOW_HEIGHT - 62, 70, 21), 0, 0);

	textures[REWARDS]->renderFrame(SDL_FRect(posX - 50, WINDOW_HEIGHT - 70, 45, 35), 4, frameLetras);
	textures[REWARDS]->renderFrame(SDL_FRect(posX, WINDOW_HEIGHT - 70, 45, 35), 1, frameLetras);
	textures[REWARDS]->renderFrame(SDL_FRect(posX + 50, WINDOW_HEIGHT - 70, 45, 35), 3, frameLetras);
	textures[REWARDS]->renderFrame(SDL_FRect(posX + 100, WINDOW_HEIGHT - 70, 45, 35), 1, frameLetras);
	textures[REWARDS]->renderFrame(SDL_FRect(posX + 150, WINDOW_HEIGHT - 70, 45, 35), 8, frameLetras);
}
void Game::RenderPasoNivel()const 
{
	// Render normal de fondo
	render();

	float posX = WINDOW_WIDTH / 2;
	float posY = WINDOW_HEIGHT / 2 - 30;

	// === S E T   L E B E L === (No hay V)
	textures[REWARDS]->renderFrame(SDL_FRect(posX - 345, posY, 72, 56), 3, 0);
	textures[REWARDS]->renderFrame(SDL_FRect(posX - 265, posY, 72, 56), 1, 0);
	textures[REWARDS]->renderFrame(SDL_FRect(posX - 185, posY, 72, 56), 8, 0);
	textures[REWARDS]->renderFrame(SDL_FRect(posX - 40, posY, 72, 56), 0, 0);
	textures[REWARDS]->renderFrame(SDL_FRect(posX + 40, posY, 72, 56), 1, 0);
	textures[REWARDS]->renderFrame(SDL_FRect(posX + 120, posY, 72, 56), 7, 0);
	textures[REWARDS]->renderFrame(SDL_FRect(posX + 200, posY, 72, 56), 1, 0);
	textures[REWARDS]->renderFrame(SDL_FRect(posX + 280, posY, 72, 56), 0, 0);

	SDL_RenderPresent(renderer);
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

	// === Render derrota ===
	if (pierde) RenderDerrota();

	SDL_RenderPresent(renderer);
}
void Game::UpdateBallons() {

	for (auto it = balloons.begin(); it != balloons.end(); )
	{
		bool destroy = (*it)->update(delta);
		bool fueraPantalla = (*it)->fueraPantalla();
		// Si fue explotado
		if (destroy)
		{
			//TODO hay que pensar como hacer que el globo no se destuya hasta que se termine la animacion
			//Realizamos la animacion de ser destruido
			(*it)->render();
			aumentaPuntuacion();
			delete* it;				 // Liberamos la memoria del globo
			it = balloons.erase(it); // Lo eliminamos del vector y actualizamos el iterador

		}
		// Si se salió de la pantalla
		else if (fueraPantalla)
		{
			(*it)->colocaGlobo(700); // Lo volvemos a colocar debajo de la pantalla
			++it;
		}
		else ++it;
	}
}
void Game::UpdateArrows() {
	for (auto it = arrows.begin(); it != arrows.end(); )
	{
		if ((*it)->update(delta)) // Si salió de la pantalla y devuelve true
		{
			delete* it;            // Liberamos la memoria de la flecha
			it = arrows.erase(it); // La eliminamos del vector y actualizamos el iterador
		}
		else
		{
			++it;
		}
	}
}
void Game::update()
{
	
	//delta -= Tiempo de ahora
	delta = (float)SDL_GetTicks()- delta;
	//actualizamos el arco
	bow->update(delta);

	// Actualizar y limpiar flechas que salen de la pantalla
	UpdateArrows();

	// Actualizar y destruir los globos explotados
	UpdateBallons();

	// Genera globos con intervalo de tiempo
	BalloonGeneration();

	// delta = tiempo de ahora
	delta = (float)SDL_GetTicks();

	// Cambio de frame de letras
	if (SDL_GetTicks() - frameAnteriorLetras > 250.0f)
	{
		frameLetras++;
		if (frameLetras >= textures[REWARDS]->getNumColumns()) frameLetras = 0;
		frameAnteriorLetras = (float)SDL_GetTicks();
	}
}

void
Game::run()
{
	while (!exit) 
	{
		handleEvents();
		update();
		render();
		compruebaDerrota();
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
		else 
		{
			// Para Cambiar la velocidad de la flecha en funcion de cuanto tiempo esta cargada
			if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_LEFT && !bow->isCharged()) cargaArco = (float)SDL_GetTicks();
			
			// Manejo de eventos del arco
			bow->handleEvent(event);
			
			// Disparo de flechas
			if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_RIGHT && bow->Shoot())
			{
				disparaFlecha();
			}

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
				points = 0;
				goal = -1;
				pierde = false;
				level = 1;
				// Volvemos a cargar el nivel
				leeArchivoNivel(level);
			}
		}
	}
}

bool Game::checkCollision(const Rectangle& rect)
{
	bool golpea = false;
	for (auto it = balloons.begin(); it != balloons.end(); )
	{
		golpea = (*it)->hit(rect);
		++it;
	}
	return golpea;
}

void Game::aumentaPuntuacion() 
{
	points += balpoints;
	if (goal > 0 && points >= goal) 
	{

		// --- Indica que se ha completado el nivel
		RenderPasoNivel();

		// Liberar todas las flechas restantes en el vector
		for (Arrow* a : arrows) {
			delete a;
		}
		arrows.clear(); 

		// --- Carga siguiente nivel
		pierde = false;
		points = 0;
		goal = -1;
		level += 1;
		leeArchivoNivel(level);

		// --- Espera 3s
		SDL_Delay(3000);
	}
}

void Game::compruebaDerrota() 
{
	if (bow->getNArrows() == 0 && !bow->isCharged() && arrows.size() == 0 && ((points < goal && goal > 0) || goal < 0))
	{
		pierde = true;
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
int Game::getFrameLetras() const
{
	return frameLetras;
}
