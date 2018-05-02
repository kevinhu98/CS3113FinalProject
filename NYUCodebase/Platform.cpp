#include "Entity.h"
#include "Shooter.h"
#include "ShaderProgram.h"
#include "Platform.h"

#define TILESIZE 0.2f

Platform::Platform() {}

Platform::Platform(float x, float y, float travelDistanceX, float travelDistanceY, int textureID) :
	travelDistanceX(travelDistanceX), travelDistanceY(travelDistanceY), maxDistanceX(x+travelDistanceX), maxDistanceY(y+travelDistanceY){
	y_acceleration = 0.0;
	x_acceleration = 0.0;
	x_velocity = 0.0;
	y_velocity = 0.0;
	x_pos = x;
	y_pos = y;

	type = PLATFORM;
	this->sprite = new SheetSprite(textureID, 11, 5, 4, 1.0f, TILESIZE);
	width = sprite->aspect * sprite->size;
	height = sprite->size;
}

