#ifndef Shooter_h
#define Shooter_h

#include <vector>
#include <stdio.h>
#include "SheetSprite.h"
#include "Bullet.h"

#define MAX_BULLETS 50

enum ShooterType { RED, GREEN, YELLOW };
enum ShooterDirection { UP, DOWN, LEFT, RIGHT };

class Shooter: public Entity {
public: 
	Shooter();
	Shooter(float x, float y, ShooterType color, ShooterDirection ShooterDirection, int textureID);
	//~Shooter();
	void shootBullet(float x_pos, float y_pos, float x_velocity, float y_velocity);
	void update(float elapsed);
	void render(ShaderProgram& program) override;
	float accum = 0;
	float shootInterval;
	int bulletIndex = 0;
	
	std::vector<Bullet> bullets;

	ShooterType color;
	ShooterDirection direction;
};

#endif