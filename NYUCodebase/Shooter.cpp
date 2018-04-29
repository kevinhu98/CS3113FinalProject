#include "Entity.h"
#include "Shooter.h"
#include "ShaderProgram.h"

ShooterType::ShooterType() {}

ShooterType::ShooterType(int ShooterType, int ShooterDirection) :
	ShooterType(ShooterType), ShooterDirection(ShooterDirection), direction(DIRECTION_UP) {

	if (ShooterType == SHOOTER_RED) {
		shootSpeed = 0.5;
	}
	else if (ShooterType == SHOOTER_YELLOW) {
		shootSpeed = 1.0;
	}
	else { shootSpeed = 1.5;  }
}


