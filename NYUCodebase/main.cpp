#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <SDL_mixer.h>
#include "SheetSprite.h"
#include "Entity.h"
#include "GameState.h"
#include "FlareMap.h"
#include "GameUtilities.h"
#include "MainMenu.h"
#include "mode.h"
#include "GameOverMenu.h"

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6

SDL_Window* displayWindow;

GameMode mode;

ShaderProgram program;
Matrix projectionMatrix;
Matrix modelMatrix;
Matrix viewMatrix;
GLuint spriteSheetTexture;
GLuint fontTexture;
const Uint8 *keys = SDL_GetKeyboardState(NULL);
SDL_Event event;
GameState gameState;
MainMenu menu;
GameOverMenu gameOver;
bool done = false;
FlareMap map;
GameUtilities Utilities;

std::map<std::string, Mix_Chunk*> sounds;

void LoadSounds() {
	Mix_Music* music = Mix_LoadMUS("finalProjectMusic2.mp3");
	sounds["jump"] = Mix_LoadWAV("playerJump.wav");
	sounds["death"] = Mix_LoadWAV("playerDeath.wav");
	//sounds["walk"] = Mix_LoadWAV("playerWalk.wav");
	Mix_VolumeChunk(sounds["jump"], 85);
	Mix_PlayMusic(music, -1);
}

int main(int argc, char *argv[]){

	float width = 1280;
	float length = 720;
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Platformer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, length, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
	glewInit();
#endif
	glViewport(0, 0, width, length);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	program.Load(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");

	projectionMatrix.SetOrthoProjection(-3.55f, 3.55f, -2.0f, 2.0f, -1.0f, 1.0f);
	program.SetProjectionMatrix(projectionMatrix);
	program.SetViewMatrix(viewMatrix);

	spriteSheetTexture = LoadTexture(RESOURCE_FOLDER"QB.png", GL_NEAREST);
	fontTexture = LoadTexture(RESOURCE_FOLDER"font.png", GL_NEAREST);

	mode = STATE_MAIN_MENU; // to load level 1, set gamestate to main menu
	Utilities.event = &event;
	Utilities.keys = keys;
	
	Utilities.shader = &program;
	Utilities.displayWindow = displayWindow;
	Utilities.done = &done;
	Utilities.spriteSheets.push_back(spriteSheetTexture);
	gameState.Initialize(&Utilities);
	menu.Initialize(&Utilities, fontTexture);
	gameOver.Initialize(&Utilities, fontTexture);

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
	LoadSounds();

	float lastFrameTicks = 0.0f;
	float accumulator = 0.0f;

	while (!done) {
		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;
		
		switch (mode) {
			case STATE_MAIN_MENU:
				menu.ProcessInput();
				break;
			case STATE_GAME_LEVEL_1:
			case STATE_GAME_LEVEL_2:
			case STATE_GAME_LEVEL_3:
				gameState.ProcessInput();
				break;
			case STATE_GAME_OVER:
				gameOver.ProcessInput();
				break;
		}

		elapsed += accumulator;
		if (elapsed < FIXED_TIMESTEP) {
			accumulator = elapsed;
			continue;	
		}

		while (elapsed >= FIXED_TIMESTEP) {

			switch (mode) {
			case STATE_MAIN_MENU:
				menu.Update(FIXED_TIMESTEP);
				break;
			case STATE_GAME_LEVEL_1:
			case STATE_GAME_LEVEL_2:
			case STATE_GAME_LEVEL_3:
				gameState.Update(FIXED_TIMESTEP);
				break;
			case STATE_GAME_OVER:
				gameOver.Update(FIXED_TIMESTEP);
				break; 
			}

			elapsed -= FIXED_TIMESTEP;
		}
		accumulator = elapsed;
		
		glClear(GL_COLOR_BUFFER_BIT);
		
		switch (mode) {
		case STATE_MAIN_MENU:
			gameState.Render();
			menu.Render();
			break;
		case STATE_GAME_LEVEL_1:
		case STATE_GAME_LEVEL_2:
		case STATE_GAME_LEVEL_3:
			gameState.Render();
			break;
		case STATE_GAME_OVER:
			gameOver.Render();
			break;
		}

		SDL_GL_SwapWindow(displayWindow);
	}

	SDL_Quit();
	return 0;
}

