#include "GameOverMenu.h"
#include "mode.h"

void GameOverMenu::ProcessInput() {
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
		}
	}

}

void GameOverMenu::Update(float elapsed) {
	//Animation
	animationTime += elapsed;
	animationValue = mapValue(animationTime, 0.0, 2.0, 0.0, 1.0);
	float animationValueB = mapValue(animationTime, 0.0, 3.0, 0.0, 1.0);
	float animationValueC = mapValue(animationTime, 0.0, 2.3, 0.0, 1.0);
	float animationValueD = mapValue(animationTime, 0.0, 2.7, 0.0, 1.0);
	float animationValueE = mapValue(animationTime, 0.0, 1.7, 0.0, 1.0);

	y_pos = easeOutElastic(3.0, 1.00, animationValue);
	y_pos2 = easeOutElastic(3.0, 1.00, animationValueB);
	y_pos3 = easeOutElastic(3.0, 1.00, animationValueC);
	y_pos4 = easeOutElastic(3.0, 1.00, animationValueD);
	x_pos = easeOutElastic(-3.5, -2.25, animationValueB);

	rotation = easeOutElastic(rotation, 0.0f, elapsed * 0.4);

}

void GameOverMenu::Render() {
	ShaderProgram& program = *Utilities->shader;

	modelMatrix.Identity();
	viewMatrix.Identity();
	//modelMatrix.Rotate(rotation);
	program.SetModelMatrix(modelMatrix);
	program.SetViewMatrix(viewMatrix);


	DrawText(&program, modelMatrix, fontTexture, "C", 0.4f, 0.20f, x_pos + 0.75, y_pos);
	DrawText(&program, modelMatrix, fontTexture, "o", 0.4f, 0.20f, x_pos + 0.95, y_pos2);
	DrawText(&program, modelMatrix, fontTexture, "n", 0.4f, 0.20f, x_pos + 1.15, y_pos4);
	DrawText(&program, modelMatrix, fontTexture, "g", 0.4f, 0.20f, x_pos + 1.35, y_pos3);
	DrawText(&program, modelMatrix, fontTexture, "r", 0.4f, 0.20f, x_pos + 1.55, y_pos4);
	DrawText(&program, modelMatrix, fontTexture, "a", 0.4f, 0.20f, x_pos + 1.75, y_pos3);
	DrawText(&program, modelMatrix, fontTexture, "t", 0.4f, 0.20f, x_pos + 1.95, y_pos4);
	DrawText(&program, modelMatrix, fontTexture, "u", 0.4f, 0.20f, x_pos + 2.15, y_pos3);
	DrawText(&program, modelMatrix, fontTexture, "l", 0.4f, 0.20f, x_pos + 2.35, y_pos4);
	DrawText(&program, modelMatrix, fontTexture, "a", 0.4f, 0.20f, x_pos + 2.55, y_pos2);
	DrawText(&program, modelMatrix, fontTexture, "t", 0.4f, 0.20f, x_pos + 2.75, y_pos2);
	DrawText(&program, modelMatrix, fontTexture, "i", 0.4f, 0.20f, x_pos + 2.95, y_pos4);
	DrawText(&program, modelMatrix, fontTexture, "o", 0.4f, 0.20f, x_pos + 3.15, y_pos3);
	DrawText(&program, modelMatrix, fontTexture, "n", 0.4f, 0.20f, x_pos + 3.35, y_pos2);
	DrawText(&program, modelMatrix, fontTexture, "s", 0.4f, 0.20f, x_pos + 3.55, y_pos4);
	DrawText(&program, modelMatrix, fontTexture, "!", 0.4f, 0.20f, x_pos + 3.75, y_pos3);


	DrawText(&program, modelMatrix, fontTexture, "Y", 0.4f, -0.20f, x_pos + 1.5, y_pos - 0.75);
	DrawText(&program, modelMatrix, fontTexture, "o", 0.4f, -0.20f, x_pos + 1.7, y_pos2 - 0.75);
	DrawText(&program, modelMatrix, fontTexture, "u", 0.4f, -0.20f, x_pos + 1.9, y_pos4 - 0.75);
	DrawText(&program, modelMatrix, fontTexture, " ", 0.4f, -0.20f, x_pos + 2.1, y_pos3 - 0.75);
	DrawText(&program, modelMatrix, fontTexture, "w", 0.4f, -0.20f, x_pos + 2.3, y_pos - 0.75);
	DrawText(&program, modelMatrix, fontTexture, "o", 0.4f, -0.20f, x_pos + 2.5, y_pos3 - 0.75);
	DrawText(&program, modelMatrix, fontTexture, "n", 0.4f, -0.20f, x_pos + 2.7, y_pos4 - 0.75);
	DrawText(&program, modelMatrix, fontTexture, "!", 0.4f, -0.20f, x_pos + 2.9, y_pos2 - 0.75);
	
}