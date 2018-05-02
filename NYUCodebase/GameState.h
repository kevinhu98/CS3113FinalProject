#ifndef GameState_h
#define GameState_h

#include <stdio.h>
#include "Bullet.h"
#include "SheetSprite.h"
#include "FlareMap.h"
#include "GameUtilities.h"
#include "Entity.h"
#include "ShaderProgram.h"

class GameState {
public:
	GameState();
	void Initialize(GameUtilities* utilities, FlareMap* map);
	void Render();
	void Update(float elapsed);
	void ProcessInput();
	//void Reset();
	void placeEntity(std::string type, float x, float y);
	void ApplyPhysics(Entity& entity, float elapsed, EntityType type);
	
	void checkBulletCollisionMap(Bullet& bullet);

	void CollideWithMapX(Entity& entity);
	void CollideWithMapY(Entity& entity);

	bool ResolveCollisionInX(Entity& entity, int x, int y, float size);
	bool ResolveCollisionInY(Entity& entity, int x, int y, float size);

	void CheckForTurn(Entity& entity);
	void CheckForJump(Entity& entity);

	void resetPlayer();
	Entity* player;
	std::vector<Entity*> entities;
	FlareMap* map;

	Matrix modelMatrix;
	Matrix viewMatrix;

	GameUtilities* Utilities;

}; 

#endif
