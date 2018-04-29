#include "MainMenu.h"
#include "mode.h"


MainMenu::MainMenu() {}

void MainMenu::Initialize(GameUtilities *utilities, GLuint fontTexture) {
	this->fontTexture = fontTexture;
	this->Utilities = utilities;
}

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
	viewMatrix.Identity();
	program.SetModelMatrix(modelMatrix);
	program.SetViewMatrix(viewMatrix);


	DrawText(&program, modelMatrix, fontTexture, "Game Programming Final", 0.4f, -0.20f, -2.0f, 1.5f);
	DrawText(&program, modelMatrix, fontTexture, "Press SPACE to Start", 0.25f, -0.15f, -1.0f, 1.0f);

}