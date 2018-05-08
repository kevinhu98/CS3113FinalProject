#include "GameOverMenu.h"
#include "mode.h"

void GameOverMenu::ProcessInput() {


}

void GameOverMenu::Update(float elapsed) {
	//Animation
	animationTime += elapsed;
	animationValue = mapValue(animationTime, 0.0, 2.0, 0.0, 1.0);
	y_pos = easeOutElastic(3.0, 1.75, animationValue);

	rotation = easeOutElastic(rotation, 0.0f, elapsed * 0.4);

}

void GameOverMenu::Render() {
	ShaderProgram& program = *Utilities->shader;

	modelMatrix.Identity();
	viewMatrix.Identity();
	//modelMatrix.Rotate(rotation);
	program.SetModelMatrix(modelMatrix);
	program.SetViewMatrix(viewMatrix);


	DrawText(&program, modelMatrix, fontTexture, "Congratulations! You won!", 0.4f, -0.20f, x_pos, y_pos);
	//DrawText(&program, modelMatrix, fontTexture, "Press SPACE to Start", 0.25f, -0.15f, x_pos + 1.0f, y_pos - 0.75);

}