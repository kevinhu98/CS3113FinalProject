#ifndef Shooter_h
#define Shooter_h

#include <vector>
#include <stdio.h>
#include "SheetSprite.h"

enum ShooterType { SHOOTER_RED, SHOOTER_GREEN, SHOOTER_YELLOW };
enum ShooterDirection { UP, DOWN, LEFT, RIGHT };

class Shooter: public Entity {
public: 
	Shooter();
	Shooter(int ShooterType, int ShooterDirection);
	~Shooter();

	float shootSpeed;
	ShooterType shooterType;
	ShooterDirection direction;
	
}

#endif