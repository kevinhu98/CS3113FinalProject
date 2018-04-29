#include "Entity.h"
#include "Shooter.h"
#include "ShaderProgram.h"

Shooter::Shooter() {}

Shooter::Shooter(ShooterType type, ShooterDirection ShooterDirection) :
	type(type), direction(ShooterDirection) {

	if (type == SHOOTER_RED) {
		shootInterval = 0.5;
	}
	else if (type == SHOOTER_YELLOW) {
		shootInterval = 1.0;
	}
	else { shootInterval = 1.5;}
	
	x_acceleration = 9.8; // cancels out gravity
}


