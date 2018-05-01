#include "Entity.h"
#include "Shooter.h"
#include "ShaderProgram.h"
#include "Bullet.h"
#include <map>
#define TILESIZE 0.2f
#define BULLET_SPEED 0.5f

extern ShaderProgram program;

std::map<ShooterType, float> shootIntervals = {
	{RED, 1.1},
	{YELLOW, 2},
	{GREEN, 3}
};

Shooter::Shooter() {}

Shooter::Shooter(float x, float y, ShooterType color, ShooterDirection ShooterDirection, int textureID) :
	color(color), direction(ShooterDirection) {
	y_acceleration = 9.8;
	x_pos = x;
	y_pos = y;
	this->type = SHOOTER;
	for (int i = 0; i < MAX_BULLETS; ++i) {
		bullets.emplace_back(0, 0, 0, 0);
		bullets.back().sprite = new SheetSprite(textureID, 13, 5, 4, 1.0f, TILESIZE * 0.25);
	}
	if (color == GREEN && ShooterDirection == DOWN) {
		this->sprite = new SheetSprite(textureID, 6, 5, 4, 1.0f, TILESIZE);
	}
	else if (color == YELLOW && ShooterDirection == DOWN) {
		this->sprite = new SheetSprite(textureID, 18, 5, 4, 1.0f, TILESIZE);
	}
	else if (color == RED && ShooterDirection == DOWN) {
		this->sprite = new SheetSprite(textureID, 13, 5, 4, 1.0f, TILESIZE);
	}
	else if (color == GREEN && ShooterDirection == RIGHT) {
		this->sprite = new SheetSprite(textureID, 8, 5, 4, 1.0f, TILESIZE);
	}
	else if (color == YELLOW && ShooterDirection == RIGHT) {
		this->sprite = new SheetSprite(textureID, 9, 5, 4, 1.0f, TILESIZE);
	}
	else if (color == RED && ShooterDirection == RIGHT) {
		this->sprite = new SheetSprite(textureID, 16, 5, 4, 1.0f, TILESIZE);
	}
	else if (color == GREEN && ShooterDirection == LEFT) {
		this->sprite = new SheetSprite(textureID, 7, 5, 4, 1.0f, TILESIZE);
	}
	else if (color == YELLOW && ShooterDirection == LEFT) {
		this->sprite = new SheetSprite(textureID, 4, 5, 4, 1.0f, TILESIZE);
	}
	else if (color == RED && ShooterDirection == LEFT) {
		this->sprite = new SheetSprite(textureID, 15, 5, 4, 1.0f, TILESIZE);
	}
	else if (color == GREEN && ShooterDirection == UP) {
		this->sprite = new SheetSprite(textureID, 10, 5, 4, 1.0f, TILESIZE);
	}
	else if (color == YELLOW && ShooterDirection == UP) {
		this->sprite = new SheetSprite(textureID, 14, 5, 4, 1.0f, TILESIZE);
	}
	else if (color == RED && ShooterDirection == UP) {
		this->sprite = new SheetSprite(textureID, 17, 5, 4, 1.0f, TILESIZE);
	}

	width = sprite->aspect * sprite->size;
	height = sprite->size;
	shootInterval = shootIntervals[color];
}

void Shooter::shootBullet(float x_pos, float y_pos, float x_velocity, float y_velocity) {
	bullets[bulletIndex].alive = true;
	bullets[bulletIndex].x_pos = x_pos;
	bullets[bulletIndex].y_pos = y_pos;
	bullets[bulletIndex].x_velocity = x_velocity;
	bullets[bulletIndex].y_velocity = y_velocity;
	++bulletIndex;
	if (bulletIndex > MAX_BULLETS - 1) {
		bulletIndex = 0;
	}
}

void Shooter::update(float elapsed) {
	for (size_t i = 0; i < bullets.size(); ++i) {
		if (bullets[i].alive == true) {
			bullets[i].update(elapsed);
		}
	}
	if (accum > shootInterval) {
		accum -= shootInterval;
		if (direction == DOWN) {
			shootBullet(x_pos, y_pos - height / 2, 0.0f, -BULLET_SPEED);
		}
		else if (direction == LEFT) {
			shootBullet(x_pos - width / 2, y_pos, -BULLET_SPEED, 0.0f);
		}
		else if (direction == RIGHT) {
			shootBullet(x_pos + width / 2, y_pos, BULLET_SPEED, 0.0f);
		}
		else if (direction == UP) {
			shootBullet(x_pos, y_pos + height / 2, 0.0f, BULLET_SPEED);
		}
	}
	else {
		accum += elapsed;
	}
}

void Shooter::render(ShaderProgram& program) {
	Entity::render(program);
	
	for (size_t i = 0; i < bullets.size(); i++) {
		bullets[i].Draw(program);
	}
}








