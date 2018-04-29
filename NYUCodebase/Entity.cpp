#include "Entity.h"
#include "ShaderProgram.h"

#define DELTA 0.0001f

Entity::Entity() {}

Entity::~Entity() {
	if (this->sprite != nullptr) {
		delete this->sprite;
	}
}

Entity::Entity(float x_pos, float y_pos, const SheetSprite* sprite, EntityType type) :
	x_pos(x_pos), y_pos(y_pos), type(type), x_velocity(0), y_velocity(0) {
	width = sprite->aspect * sprite->size;
	height = sprite->size;

	this->sprite = new SheetSprite(*sprite);
	
}

void Entity::setSprite(SheetSprite* newSprite) {
	sprite = newSprite;
	float aspect = newSprite->width / newSprite->height;
	width = aspect * newSprite->size;
	height = newSprite->size;
}

void Entity::render(ShaderProgram &program) {
	modelMatrix.Identity();
	modelMatrix.Translate(x_pos, y_pos, 0.0f);
	program.SetModelMatrix(modelMatrix);

	if ((sprite->reversedImage && x_velocity > 0) ||
		(!sprite->reversedImage && x_velocity < 0)) {
		sprite->reversedImage = !sprite->reversedImage;
	}
	
	sprite->Draw(&program);
}

void Entity::update(float elapsed) {
	collidedBottom = false;
	collidedLeft = false;
	collidedRight = false;
	collidedTop = false;
}

bool Entity::collisionEntity(Entity* other) {
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

bool Entity::collisionInX(float x, float otherWidth) {
	collidedLeft = x_pos - width / 2 < x + otherWidth / 2 && x_pos - width / 2 > x - otherWidth / 2;
	collidedRight = x_pos + width / 2 < x + otherWidth / 2 && x_pos + width / 2 > x - otherWidth / 2;
	if (collidedRight) {
		x_velocity = 0;
		float penetration = fabs((x_pos + width / 2) - (x - otherWidth/2)) ;
		x_pos -= penetration + DELTA;
	}

	if (collidedLeft) {
		x_velocity = 0;
		float penetration = fabs((x + otherWidth / 2) - (x_pos - width / 2));
		x_pos += penetration + DELTA;
	}

	return (collidedLeft || collidedRight);
}

bool Entity::collisionInY(float y, float otherHeight) {
	collidedTop = y_pos + height / 2 < y + otherHeight / 2 && y_pos + height / 2 > y - otherHeight / 2;
	collidedBottom = y_pos - height / 2  < y + otherHeight / 2 && y_pos + height / 2 > y + otherHeight / 2; 
	if (collidedTop) {
		y_velocity = 0;
		float penetration = fabs((y_pos + height / 2) - (y - otherHeight/2)) ;
		y_pos -= penetration - DELTA;
	}

	if (collidedBottom) {
		y_velocity = 0;
		float penetration = fabs((y + otherHeight / 2) - (y_pos - height / 2));
		y_pos += penetration + DELTA;
	}

	return (collidedTop || collidedBottom);
}

