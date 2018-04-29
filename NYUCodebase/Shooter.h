#ifndef Shooter_h
#define Shooter_h

#include <vector>
#include <stdio.h>
#include "SheetSprite.h"

enum ShooterType { SHOOTER_RED, SHOOTER_GREEN, SHOOTER_YELLOW };
enum ShooterDirection { DIRECTION_UP };

class Shooter: public Enitity {
public: 
	Shooter();
	Shooter(int ShooterType, int ShooterDirection);
	~Shooter();

	float shootSpeed;
	ShooterType shooterType;
	ShooterDirection direction;
	
}

#endif