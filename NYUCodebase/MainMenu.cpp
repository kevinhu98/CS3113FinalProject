#include "MainMenu.h"
#include "Main.cpp"


MainMenu::MainMenu() {}

void MainMenu::ProcessInput() {
	SDL_Event& event = *Utilities->event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			*Utilities->done = true;
		}
		else if (event.type == SDL_KEYDOWN) {
			//Alternate exit button
			if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
				*Utilities->done = true;
			}

			if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
				mode = STATE_GAME_LEVEL;
			}
		}
	}
}

void MainMenu::Update(float elapsed) {
	//Animation
}

void MainMenu::Render() {
	ShaderProgram& program = *Utilities->shader;

	modelMatrix.Identity();
	program.SetModelMatrix(modelMatrix);



}