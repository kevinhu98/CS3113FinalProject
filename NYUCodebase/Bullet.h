#ifndef Bullet_h
#define Bullet_h

#include "Entity.h"
#include <vector>
#include <stdio.h>

class Bullet {
public:
	Bullet();
	Bullet(float x_pos, float y_pos, float x_velocity, float y_velocity);

	void Draw(ShaderProgram& program);
	void update(float elapsed);
	bool checkCollisionPlayer(Entity* player);

	float x_pos;
	float y_pos;
	
	float width;
	float height;

	float x_velocity;
	float y_velocity;

	SheetSprite* sprite;
	Matrix modelMatrix;

	bool alive = false;
};


#endif // !

