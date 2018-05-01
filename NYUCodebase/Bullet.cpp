#include "Bullet.h"

Bullet::Bullet(){}

Bullet::Bullet(float x_pos, float y_pos, float x_velocity, float y_velocity) :
	x_pos(x_pos), y_pos(y_pos), height(0.05), width(0.05) {}

void Bullet::update(float elapsed) {
	x_pos += x_velocity * elapsed;
	y_pos += y_velocity * elapsed;
}

void Bullet::Draw(ShaderProgram& program) {
	if (!alive) return;
	modelMatrix.Identity();
	modelMatrix.Translate(x_pos, y_pos, 0.0f);
	program.SetModelMatrix(modelMatrix);

	sprite->Draw(&program);
}

bool Bullet::checkCollisionPlayer(Entity* other) {
	if (!alive) return false;
	if (y_pos - height / 2 > other->y_pos + other->height / 2) {
		return false;
	}
	else if (y_pos + height / 2 < other->y_pos - other->height / 2) {
		return false;
	}
	else if (x_pos - width / 2 > other->x_pos + other->width / 2) {
		return false;
	}
	else if (x_pos + width / 2 < other->x_pos - other->width / 2) {
		return false;
	}
	return true;
}

