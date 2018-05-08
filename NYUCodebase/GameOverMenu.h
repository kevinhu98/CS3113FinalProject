#ifndef GameOverMenu_h
#define GameOverMenu_h

#include <vector>
#include <stdio.h>
#include "SheetSprite.h"
#include "GameUtilities.h"
#include "ShaderProgram.h"
#include "MainMenu.h"

class GameOverMenu : public MainMenu {
public:
	void Render();
	void Update(float elapsed);
	void ProcessInput();

	float y_pos2;
	float y_pos3;
	float y_pos4;

};

#endif
