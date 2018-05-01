#ifndef Platform_h
#define Platform_h

#include "Entity.h"
#include <vector>
#include <stdio.h>
#include "SheetSprite.h"

class Platform : public Entity {
public:
	Platform();
	Platform(float x, float y, float travelDistanceX, float travelDistanceY, int textureID);
	//~Platform();
	
	float travelDistanceX;
	float travelDistanceY;

	float maxDistanceX;
	float maxDistanceY;
};

#endif // !Header_h
