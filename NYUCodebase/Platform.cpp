#include "Entity.h"
#include "Shooter.h"
#include "ShaderProgram.h"
#include "Platform.h"
#define TILESIZE 0.2f

Platform::Platform() {}

Platform::Platform(float x, float y, float travelDistanceX, float travelDistanceY, int textureID) :
	travelDistanceX(travelDistanceX), travelDistanceY(travelDistanceY), maxDistanceX(x+travelDistanceX), maxDistanceY(y+travelDistanceY){
	y_acceleration = 9.8;
	x_velocity = travelDistanceX / x;
	y_velocity = travelDistanceY / y;
	x_pos = x;
	y_pos = y;

	this->sprite = new SheetSprite(textureID, 11, 5, 4, 1.0f, TILESIZE);
	width = sprite->aspect * sprite->size;
	height = sprite->size;
}