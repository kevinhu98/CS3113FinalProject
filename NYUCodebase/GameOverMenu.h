#ifndef GameOverMenu_h
#define GameOverMenu_h

#include <vector>
#include <stdio.h>
#include "SheetSprite.h"
#include "GameUtilities.h"
#include "ShaderProgram.h"

class GameOverMenu : public MainMenu {
	void Render();
	void Update(float elapsed);
	void ProcessInput();


};
