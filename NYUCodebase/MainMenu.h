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


	Matrix modelMatrix;
	Matrix viewMatrix;

	GLuint fontTexture; 
	GameUtilities* Utilities;

};
#endif // !MainMenu_h


