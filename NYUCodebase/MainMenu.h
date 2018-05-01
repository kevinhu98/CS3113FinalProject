#ifndef MainMenu_h
#define MainMenu_h

#include <vector>
#include <stdio.h>
#include "SheetSprite.h"
#include "GameUtilities.h"
#include "ShaderProgram.h"

class MainMenu {
public: 
	MainMenu();
	void Initialize(GameUtilities* utilities, GLuint fontTexture);
	void Render();
	void Update(float elapsed);
	void ProcessInput();

	float x_pos;
	float y_pos;
	float x_velocity;
	float y_velocity;
	float x_acceleration;
	float y_acceleration;


	Matrix modelMatrix;
	Matrix viewMatrix;

	GLuint fontTexture; 
	GameUtilities* Utilities;

};
#endif // !MainMenu_h


