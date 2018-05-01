#include "GameState.h"
#include "Shooter.h"
#include "Platform.h"
#include <unordered_set>

#define GRAVITY 9.8f
#define ACCELERATION 0.5f
#define FRICTION 0.99f
#define MAX_BULLETS 100

GameState::GameState() {}
//std::unordered_set<int> solidTiles = { 0,1,17,33,34 };
std::unordered_set<int> solidTiles = {11,12};

void GameState::Initialize(GameUtilities* utilities, FlareMap* map) {
	this->Utilities = utilities;
	this->map = map;
	//sheetSprites.emplace_back(map->spriteSheetTexture, , map->spritesX, map->spritesY, 1.0f, map->tileSize);
	for (size_t i = 0; i < map->entities.size(); i++) {
		placeEntity(map->entities[i].type, map->entities[i].x * map->tileSize + map->tileSize / 2, -map->entities[i].y * map->tileSize + map->tileSize / 2);
	}
}


void GameState::placeEntity(std::string type, float x, float y) {
	if (type == "PLAYER") {
		SheetSprite sprite(map->spriteSheetTexture, 2, 5, 4, 1.0f, map->tileSize);
		player = new Entity(x, y, &sprite, PLAYER);
		entities.push_back(player);
	}
	else if (type == "CHASER") {		
		SheetSprite sprite(map->spriteSheetTexture, 5, 5, 4, 1.0f, map->tileSize);
		Entity* enemy = new Entity(x, y, &sprite, ENEMY);
		//enemy->x_acceleration = 5; //150
		enemy->x_velocity = 1;
		entities.push_back(enemy);
	}
	else if (type == "PLATFORM") {
		Platform* platform = new Platform(x, y, 0.5, 0, map->spriteSheetTexture);
		entities.push_back(platform);
	}
	else if (type == "GREEN_DOWN") {
		Shooter* enemy = new Shooter(x, y, GREEN, DOWN, map->spriteSheetTexture);//should be shooter
		entities.push_back(enemy);
	}
}

void GameState::ProcessInput() {
	SDL_Event& event = *Utilities->event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			*Utilities->done = true;
		}
		else if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.scancode == SDL_SCANCODE_SPACE && player->collidedBottom) { // jump
				player->y_velocity = 4.0f;
			}
			//Alternate exit button
			if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
				*Utilities->done = true;
			}
		}
		else if (event.type == SDL_KEYUP) {
			if (event.key.keysym.scancode == SDL_SCANCODE_UP) {
				player->x_velocity = 0;
			}
			else if (event.key.keysym.scancode == SDL_SCANCODE_LEFT || event.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
				player->x_velocity = 0;
			}
		}
	}
	const Uint8* keys = Utilities->keys;

	if (true) { //player->collidedBottom
		if (keys[SDL_SCANCODE_LEFT]) {
			//player->x_acceleration = -0.7f;
			player->x_velocity = -1.0;
		}
		else if (keys[SDL_SCANCODE_RIGHT]) {
			//player->x_acceleration = 0.7f;
			player->x_velocity = 1.0;

		}
	}
	else {
		if (keys[SDL_SCANCODE_RIGHT]) {
		//	player->x_velocity
		}
		else if (keys[SDL_SCANCODE_LEFT]) {
			//player->x_acceleration = -1.0f;
		}
	}	
}

void GameState::ApplyPhysics(Entity& entity, float elapsed){
	entity.update(elapsed); //reset all collision flags

	//friction
	//entity.x_velocity = lerp(entity.x_velocity, 0.0f, elapsed * FRICTION);
	//entity.y_velocity = lerp(entity.y_velocity, 0.0f, elapsed * FRICTION);

	//acceleration
	entity.x_velocity += entity.x_acceleration * elapsed;
	entity.y_velocity += entity.y_acceleration * elapsed;

	//gravity
	entity.y_velocity -= elapsed * GRAVITY;

	//x-velo
	entity.x_pos += entity.x_velocity * elapsed;
	CollideWithMapX(entity);

	//y-velo
	entity.y_pos += entity.y_velocity * elapsed;
	CollideWithMapY(entity);
}

void GameState::Update(float elapsed) {
	//updates only occur when player moves
	if (player->x_velocity < 0.1 && player->x_velocity > -0.1 
		&& player->y_velocity < 0.1 && player->y_velocity > -0.1) {
		ApplyPhysics(*player, elapsed);
		return;
	}

	for (size_t i = 0; i < entities.size(); ++i) {
		ApplyPhysics(*entities[i], elapsed); //enemy movement
	}

	for (size_t i = 0; i < entities.size(); ++i){
		if (entities[i]->type == ENEMY) { // AI
			CheckForTurn(*entities[i]);
			CheckForJump(*entities[i]);
		}

		//death when touching enemy
		if (player->collisionEntity(entities[i]) && entities[i]->type == ENEMY) {
			resetPlayer();
		}

		if (entities[i]->type == SHOOTER) {
			Shooter* shooter = ((Shooter*)entities[i]);
			for (size_t i = 0; i < shooter->bullets.size(); ++i) {
				if (shooter->bullets[i].checkCollisionPlayer(player)) {
					resetPlayer();
				}
			}
		}

		for (size_t j = 0; j < entities.size(); ++j) { // enemies bounce off each other
			if (entities[j]->collisionEntity(entities[i]) == true
				&& entities[i]->type == ENEMY && entities[j]->type == ENEMY) {
				entities[i]->x_velocity *= -1;
				entities[j]->x_velocity *= -1;
			}
		}
	}
	//bounds check + keeps in bounds
	if (player->x_pos - player->width / 2 < 0) { //left bound
		player->x_velocity = 0;
		player->x_pos = map->tileSize / 2;
	}
	else if (player->x_pos + player->width / 2 > (map->tileSize*map->mapWidth)) { //right bound
		player->x_velocity = 0;
		player->x_pos = map->tileSize*map->mapWidth - map->tileSize / 2;
	}
	
}

void GameState::Render() {
	ShaderProgram& program = *Utilities->shader;
	modelMatrix.Identity();
	program.SetModelMatrix(modelMatrix);

	//transitions screen
	viewMatrix.Identity();
	if (player) {
		viewMatrix.Translate(-player->x_pos, -player->y_pos, 0.0f);
		program.SetViewMatrix(viewMatrix);
	}
	//render map
	map->render(program);

	//render entities
	for (size_t i = 0; i < entities.size(); ++i) {
		entities[i]->render(program);
	}
}

bool GameState::ResolveCollisionInX(Entity& entity, int x, int y, float size) {
	if (x < 0 || y < 0 || map->mapData[y][x] == 0 ||
		solidTiles.find(map->mapData[y][x] - 1) == solidTiles.end()) {
		return false;
	}
	//collided tile center
	float tileCenter = (x * size) + (size / 2);
	return entity.collisionInX(tileCenter, size);
}

bool GameState::ResolveCollisionInY(Entity& entity, int x, int y, float size) {
	if (x < 0 || y < 0 || map->mapData[y][x] == 0 ||
		solidTiles.find(map->mapData[y][x] - 1) == solidTiles.end()) {
		return false;
	}
	//collided tile center
	float tileCenter = (-y * size) - (size / 2);
	return entity.collisionInY(tileCenter, size);
}

void GameState::CollideWithMapX(Entity& entity) {
	if (entity.x_velocity > 0) {
		//right collision
		int rightX, rightBottomY, rightTopY; //two points on right-line of entity
		map->worldToTileCoordinates(entity.x_pos + entity.width / 2, entity.y_pos + entity.height / 2.5, rightX, rightBottomY);
		map->worldToTileCoordinates(entity.x_pos + entity.width / 2, entity.y_pos - entity.height / 2.5, rightX, rightTopY);
		if (ResolveCollisionInX(entity, rightX, rightBottomY, map->tileSize) == false) {
			ResolveCollisionInX(entity, rightX, rightTopY, map->tileSize);
		}
	}
	else {
		//left collision
		int leftX, leftBottomY, leftTopY; //two points on left-line of entity
		map->worldToTileCoordinates(entity.x_pos - entity.width / 2, entity.y_pos + entity.height / 2.5, leftX, leftBottomY);
		map->worldToTileCoordinates(entity.x_pos - entity.width / 2, entity.y_pos - entity.height / 2.5, leftX, leftTopY);
		if (ResolveCollisionInX(entity, leftX, leftBottomY, map->tileSize) == false) {
			ResolveCollisionInX(entity, leftX, leftTopY, map->tileSize);
		}
	}
}

void GameState::CollideWithMapY(Entity& entity) {
	// near left and right edges of entity -- makes sure that entity doesnt slip off edges unintentially
	int leftEdge = map->getTileCoordinateXPos(entity.x_pos - entity.width / 2.5);
	int rightEdge = map->getTileCoordinateXPos(entity.x_pos + entity.width / 2.5);

	if (entity.y_velocity > 0) {
		int topY = map->getTileCoordinateYPos(entity.y_pos + entity.height / 2);
		if (ResolveCollisionInY(entity, leftEdge, topY, map->tileSize) == false) {
			ResolveCollisionInY(entity, rightEdge, topY, map->tileSize);

		}
	}
	else {
		int botY = map->getTileCoordinateYPos(entity.y_pos - entity.height / 2);
		if (ResolveCollisionInY(entity, leftEdge, botY, map->tileSize) == false) {
			ResolveCollisionInY(entity, rightEdge, botY, map->tileSize);

		}
	}
}

void GameState::CheckForTurn(Entity& entity) {
	if (entity.sprite->reversedImage == true) { // if entity is walking left off cliff
		int leftCheckX, leftCheckY, leftCloseX, leftCloseY;
		map->worldToTileCoordinates(entity.x_pos - entity.width, entity.y_pos - entity.height * 3, leftCheckX, leftCheckY); //cliff check
		map->worldToTileCoordinates(entity.x_pos - entity.width * 0.75, entity.y_pos + entity.height * 0.45, leftCloseX, leftCloseY); //wall check
		if (solidTiles.find(map->mapData[leftCloseY][leftCloseX] - 1) != solidTiles.end() 
			|| solidTiles.find(map->mapData[leftCheckY][leftCheckX] - 1) == solidTiles.end()) {
			entity.x_velocity *= -1;
		}
	}
	else {
		int rightCheckX, rightCheckY, rightCloseX, rightCloseY;
		map->worldToTileCoordinates(entity.x_pos + entity.width , entity.y_pos - entity.height * 3, rightCheckX, rightCheckY);
		map->worldToTileCoordinates(entity.x_pos + entity.width / 1.95, entity.y_pos + entity.width / 2.05, rightCloseX, rightCloseY);
		if (solidTiles.find(map->mapData[rightCloseY][rightCloseX] - 1) != solidTiles.end()
			|| solidTiles.find(map->mapData[rightCheckY][rightCheckX] - 1) == solidTiles.end()) {
			entity.x_velocity *= -1;
		}
	}
}

void GameState::CheckForJump(Entity& entity) {
	if (entity.sprite->reversedImage == true) {
		int leftFrontX, leftFrontY, leftTopX, leftTopY;
		map->worldToTileCoordinates(entity.x_pos - entity.width * 2, entity.y_pos, leftFrontX, leftFrontY);
		map->worldToTileCoordinates(entity.x_pos - entity.width * 2, entity.y_pos + entity.height * 2, leftTopX, leftTopY);
		if (leftFrontX < 0 || leftFrontY < 0 ||
			leftTopX < 0 || leftTopY < 0) {
			return;
		}
		if (solidTiles.find(map->mapData[leftTopY][leftTopX] - 1) == solidTiles.end() &&
			!(solidTiles.find(map->mapData[leftFrontY][leftFrontX] - 1) == solidTiles.end())) {
			entity.y_velocity = 2.5f;
		}
	}
	else{
		int rightFrontX, rightFrontY, rightTopX, rightTopY;
		map->worldToTileCoordinates(entity.x_pos + entity.width * 1.5 , entity.y_pos, rightFrontX, rightFrontY);
		map->worldToTileCoordinates(entity.x_pos + entity.width * 1.5, entity.y_pos + entity.height * 2, rightTopX, rightTopY);
	
		// Ignore invalid tile indices
		if (rightFrontX < 0 || rightFrontY < 0 || 
			rightTopX < 0 || rightTopY < 0) {
			return;
		}
		//
		if (solidTiles.find(map->mapData[rightTopY][rightTopX] - 1) == solidTiles.end() &&
			(solidTiles.find(map->mapData[rightFrontY][rightFrontX] - 1) != solidTiles.end())) {
			entity.y_velocity = 2.5f;
		}
	}
}

void GameState::resetPlayer() {
	player->x_pos = map->tileSize / 2;
	player->y_pos = -3.8;
	player->x_velocity = 0;
	player->y_velocity = 0;
}