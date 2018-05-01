#include "MainMenu.h"
#include "mode.h"


MainMenu::MainMenu() :
	x_pos(-2.0), y_pos(1.5), rotation(45), animationTime(0), animationValue(0) {}

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
	animationTime += elapsed;
	animationValue = mapValue(animationTime, 0.0, 2.0, 0.0, 1.0);
	y_pos = easeOutElastic(4.0, 1.75, animationValue);

	rotation = easeOutElastic(rotation, 0.0f, elapsed * 0.4);

}

void MainMenu::Render() {
	ShaderProgram& program = *Utilities->shader;

	modelMatrix.Identity();
	viewMatrix.Identity();
	//modelMatrix.Rotate(rotation);
	program.SetModelMatrix(modelMatrix);
	program.SetViewMatrix(viewMatrix);


	DrawText(&program, modelMatrix, fontTexture, "Game Programming Final", 0.4f, -0.20f, x_pos, y_pos);
	DrawText(&program, modelMatrix, fontTexture, "Press SPACE to Start", 0.25f, -0.15f, x_pos + 1.0f, y_pos - 0.75);
}