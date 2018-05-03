#ifndef GameUtilities_h
#define GameUtilities_h

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <SDL.h>
#include <SDL_opengl.h>
#include <vector>
#include <string>
#include <stdio.h>
#include <SDL_mixer.h>
#include <map>

extern std::map<std::string, Mix_Chunk*> sounds;

class Matrix;
class ShaderProgram;

class GameUtilities {
public:
	ShaderProgram* shader;
	const Uint8* keys;
	SDL_Event* event;
	SDL_Window* displayWindow;
	std::vector<GLuint> spriteSheets;
	bool* done;
};

float lerp(float v0, float v1, float t);
float easeIn(float from, float to, float time);
float easeOut(float from, float to, float time);
float easeOutElastic(float from, float to, float time);
float mapValue(float value, float srcMin, float srcMax, float dstMin, float dstMax);

void DrawText(ShaderProgram *program, Matrix modelMatrix, int fontTexture, std::string text, float size, float spacing, float x, float y);

GLuint LoadTexture(const char *filePath, GLuint filter);

#endif // !GameUtilities_h
