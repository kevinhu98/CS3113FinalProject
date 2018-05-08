#include "GameState.h"
#include "Shooter.h"
#include "Platform.h"

#include "mode.h"
#include <unordered_set>
#include <SDL_mixer.h>

#define RESOURCE_FOLDER ""

#define GRAVITY 9.8f
#define ACCELERATION 0.5f
#define FRICTION 0.99f
#define MAX_BULLETS 100

extern GameMode mode;
extern Entity* player;

GameState::GameState() {}

std::unordered_set<int> solidTiles = {11,12,19}; // gray and blue tiles

void GameState::Initialize(GameUtilities* utilities) {
	this->Utilities = utilities;
	LoadLevel();
}

void GameState::placeEntity(std::string type, float x, float y) {
	if (type == "PLAYER") {
		SheetSprite sprite(map->spriteSheetTexture, 2, 5, 4, 1.0f, map->tileSize);
		player = new Entity(x, y, &sprite, PLAYER);
		entities.push_back(player);
	}
	else if (type == "CHASER1") {		
		SheetSprite sprite(map->spriteSheetTexture, 5, 5, 4, 1.0f, map->tileSize);
		Entity* enemy = new Entity(x, y, &sprite, CHASER1);
		//enemy->x_acceleration = 5; //150
		enemy->x_velocity = 1;
		entities.push_back(enemy);
	}
	else if (type == "CHASER2") {
		SheetSprite sprite(map->spriteSheetTexture, 5, 5, 4, 1.0f, map->tileSize);
		Entity* enemy = new Entity(x, y, &sprite, CHASER2);
		//enemy->x_acceleration = 5; //150
		enemy->x_velocity = 0.4;
		entities.push_back(enemy);
	}
	//DOWN
	else if (type == "GREEN_DOWN") {
		Shooter* enemy = new Shooter(x, y, GREEN, DOWN, map->spriteSheetTexture);
		entities.push_back(enemy);
	}
	else if (type == "YELLOW_DOWN") {
		Shooter* enemy = new Shooter(x, y, YELLOW, DOWN, map->spriteSheetTexture);
		entities.push_back(enemy);
	}
	else if (type == "RED_DOWN") {
		Shooter* enemy = new Shooter(x, y, RED, DOWN, map->spriteSheetTexture);
		entities.push_back(enemy);
	}

	//RIGHT
	else if (type == "GREEN_RIGHT") {
		Shooter* enemy = new Shooter(x, y, GREEN, RIGHT, map->spriteSheetTexture);
		entities.push_back(enemy);
	}
	else if (type == "YELLOW_RIGHT") {
		Shooter* enemy = new Shooter(x, y, YELLOW, RIGHT, map->spriteSheetTexture);
		entities.push_back(enemy);
	}
	else if (type == "RED_RIGHT") {
		Shooter* enemy = new Shooter(x, y, RED, RIGHT, map->spriteSheetTexture);
		entities.push_back(enemy);
	}

	//LEFT
	else if (type == "GREEN_LEFT") {
		Shooter* enemy = new Shooter(x, y, GREEN, LEFT, map->spriteSheetTexture);
		entities.push_back(enemy);
	}
	else if (type == "YELLOW_LEFT") {
		Shooter* enemy = new Shooter(x, y, YELLOW, LEFT, map->spriteSheetTexture);
		entities.push_back(enemy);
	}
	else if (type == "RED_LEFT") {
		Shooter* enemy = new Shooter(x, y, RED, LEFT, map->spriteSheetTexture);
		entities.push_back(enemy);
	}

	//UP
	else if (type == "GREEN_UP") {
		Shooter* enemy = new Shooter(x, y, GREEN, UP, map->spriteSheetTexture);
		entities.push_back(enemy);
	}
	else if (type == "YELLOW_UP") {
		Shooter* enemy = new Shooter(x, y, YELLOW, UP, map->spriteSheetTexture);
		entities.push_back(enemy);
	}
	else if (type == "RED_UP") {
		Shooter* enemy = new Shooter(x, y, RED, UP, map->spriteSheetTexture);
		entities.push_back(enemy);
	}
	
}

void GameState::ProcessInput() {
	SDL_Event& event = *Utilities->event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			*Utilities->done = true;
			Mix_FreeChunk(sounds["jump"]);
			Mix_FreeChunk(sounds["walk"]);
			//Mix_FreeMusic(music);
		}
		else if (event.type == SDL_KEYDOWN) {
			if ((event.key.keysym.scancode == SDL_SCANCODE_SPACE || event.key.keysym.scancode == SDL_SCANCODE_UP)
				&& player->collidedBottom) { // jump
				player->y_velocity = 4.0f;
				Mix_PlayChannel(-1, sounds["jump"], 0);
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

	//if (player->collidedBottom) {
		if (keys[SDL_SCANCODE_LEFT]) {
			player->x_velocity = -1.0;
			//Mix_PlayChannel(-1, sounds["walk"], 0);
		}
		else if (keys[SDL_SCANCODE_RIGHT]) {
			player->x_velocity = 1.0;
			//Mix_PlayChannel(-1, sounds["walk"], 0);
		}
//	}
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
		ApplyPhysics(*entities[i], elapsed); //enemy movement and player movement

		if (entities[i]->type == CHASER1) { // AI
			CheckForTurn(*entities[i]);
			CheckForJump(*entities[i]);
		}
		else if (entities[i]->type == CHASER2) {
			CheckForTurn(*entities[i]);
		}

		for (size_t j = 0; j < entities.size(); ++j) { // enemies bounce off each other
			if (entities[i]->collisionEntity(entities[j]) == true) {
				entities[i]->x_velocity *= -1;
				entities[j]->x_velocity *= -1;
			}
		}


		//death when touching enemy
		if (player->collisionEntity(entities[i]) && (entities[i]->type == CHASER1 || entities[i]->type == CHASER2)) {
			Mix_PlayChannel(-1, sounds["death"], 0);
			resetPlayer();

		}

		//stops drawing bullets if they touch map
		if (entities[i]->type == SHOOTER) {
			Shooter* shooter = ((Shooter*)entities[i]);
			for (size_t i = 0; i < shooter->bullets.size(); ++i) {
				checkBulletCollisionMap(shooter->bullets[i]);
				if (shooter->bullets[i].checkCollisionPlayer(player)) {
					Mix_PlayChannel(-1, sounds["death"], 0);
					resetPlayer();
				}
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

		CheckForNextLevel();

}
void GameState::Render(){
	glClear(GL_COLOR_BUFFER_BIT);
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
		int leftCheckX, leftCheckY, leftCloseX, leftCloseY, leftPlatX, leftPlatY;
		map->worldToTileCoordinates(entity.x_pos - entity.width, entity.y_pos - entity.height * 3, leftCheckX, leftCheckY); //cliff check
		map->worldToTileCoordinates(entity.x_pos - entity.width / 1.95, entity.y_pos + entity.height / 2.05, leftCloseX, leftCloseY); //wall check
		map->worldToTileCoordinates(entity.x_pos - entity.width / 1.95, entity.y_pos - entity.height / 1.95, leftPlatX, leftPlatY);

		if (entity.type == CHASER2) {
			if (solidTiles.find(map->mapData[leftPlatY][leftPlatX] - 1) == solidTiles.end()) {
				entity.x_velocity *= -1;
			}
		}

		else if (entity.type == CHASER1) {
			if (solidTiles.find(map->mapData[leftCloseY][leftCloseX] - 1) != solidTiles.end()
				|| solidTiles.find(map->mapData[leftCheckY][leftCheckX] - 1) == solidTiles.end()) {
				entity.x_velocity *= -1;
			}
			
		}
	}
	else {
		int rightCheckX, rightCheckY, rightCloseX, rightCloseY, rightPlatX, rightPlatY;
		map->worldToTileCoordinates(entity.x_pos + entity.width , entity.y_pos - entity.height * 3, rightCheckX, rightCheckY);
		map->worldToTileCoordinates(entity.x_pos + entity.width / 1.95, entity.y_pos + entity.width / 2.05, rightCloseX, rightCloseY);
		map->worldToTileCoordinates(entity.x_pos + entity.width / 1.95, entity.y_pos - entity.height / 1.95, rightPlatX, rightPlatY);

		if (entity.type == CHASER2) {
			if (solidTiles.find(map->mapData[rightPlatY][rightPlatX] - 1) == solidTiles.end()) {
				entity.x_velocity *= -1;
			}
		}

		else if (entity.type == CHASER1) {
			if (solidTiles.find(map->mapData[rightCloseY][rightCloseX] - 1) != solidTiles.end()
				|| solidTiles.find(map->mapData[rightCheckY][rightCheckX] - 1) == solidTiles.end()) {
				entity.x_velocity *= -1;
			}
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
	if (mode == STATE_GAME_LEVEL_1 || STATE_MAIN_MENU) {
		player->x_pos = map->tileSize / 2;
		player->y_pos = -6;
	}
	else if (mode == STATE_GAME_LEVEL_2) {
		player->x_pos = map->tileSize / 2;
		player->y_pos = -6;
	}
	else if (mode == STATE_GAME_LEVEL_3) {
		player->x_pos = map->tileSize * 2; // should be slightly more forward
		player->y_pos = -10;
	}
	player->x_velocity = 0;
	player->y_velocity = 0;
}

void GameState::checkBulletCollisionMap(Bullet& bullet) {
	if (!bullet.alive) return;
	int botX, botY, rightX, rightY, leftX, leftY, topX, topY;
	map->worldToTileCoordinates(bullet.x_pos, bullet.y_pos + bullet.height / 2, topX, topY);
	map->worldToTileCoordinates(bullet.x_pos, bullet.y_pos - bullet.height / 2, botX, botY);
	map->worldToTileCoordinates(bullet.x_pos - bullet.width / 2, bullet.y_pos, leftX, leftY);
	map->worldToTileCoordinates(bullet.x_pos + bullet.width / 2, bullet.y_pos, rightX, rightY);
	
	//first checks if bullet is going out of bounds, prevents crash
	if (botX > 0 && botY > 0 && (solidTiles.find(map->mapData[botY][botX] - 1) != solidTiles.end()) && botY < map->mapHeight && botX < map->mapWidth) {
		bullet.alive = false;
	}
	/*
	if (leftX > 0 && leftY > 0 && (solidTiles.find(map->mapData[leftY][leftX] - 1) != solidTiles.end()) && leftY < map->mapHeight && leftX < map->mapWidth) {
		bullet.alive = false;
	}
	if (rightX > 0 && rightY > 0 && (solidTiles.find(map->mapData[rightY][rightX] - 1) != solidTiles.end()) && rightY < map->mapHeight && rightX < map->mapWidth) {
		bullet.alive = false;
	}
	if (topX > 0 && topY > 0 && (solidTiles.find(map->mapData[topY][topX] - 1) != solidTiles.end()) && topY < map->mapHeight && topX < map->mapWidth) {
		bullet.alive = false;
	}
	*/

	if (botX < 0 || botY < 0 || map->mapData[botY][botX] == 0 ||
		solidTiles.find(map->mapData[botY][botX] - 1) == solidTiles.end()) {
		return;
	}
	else if (rightX < 0 || rightY < 0 || map->mapData[rightY][rightX] == 0 ||
		solidTiles.find(map->mapData[rightY][rightX] - 1) == solidTiles.end()) {
		return;
	}
	else if (leftX < 0 || leftY < 0 || map->mapData[leftY][leftX] == 0 ||
		solidTiles.find(map->mapData[leftY][leftX] - 1) == solidTiles.end()) {
		return;
	}
	else if (topX < 0 || topY < 0 || map->mapData[topY][topX] == 0 ||
		solidTiles.find(map->mapData[topY][topX] - 1) == solidTiles.end()) {
		return;
	}
	//checks all corners of bullet for map collision

	if ((solidTiles.find(map->mapData[botY][botX] - 1) != solidTiles.end())) {
		bullet.alive = false;
	}
	else if ((solidTiles.find(map->mapData[topY][topX] - 1) != solidTiles.end())) {
		bullet.alive = false;
	}
	else if ((solidTiles.find(map->mapData[leftY][leftX] - 1) != solidTiles.end())) {
		bullet.alive = false;
	}
	else if ((solidTiles.find(map->mapData[rightY][rightX] - 1) != solidTiles.end())) {
		bullet.alive = false;
	}
	
}

void GameState::LoadLevel() {
	if (mode == STATE_GAME_LEVEL_1) {
		return;
	}
	if (mode == STATE_MAIN_MENU) {
		map = new FlareMap();
		map->Load(RESOURCE_FOLDER"finalproject1.txt");
	}
	else if (mode == STATE_GAME_LEVEL_2) {
		delete map;
		map = new FlareMap();
		map->Load(RESOURCE_FOLDER"finalproject2.txt");
	}
	else if (mode == STATE_GAME_LEVEL_3) {
		delete map;
		map = new FlareMap();
		map->Load(RESOURCE_FOLDER"finalproject3.txt");
	}
	map->setSpriteSheet(Utilities->spriteSheets[0], 5, 4);

	// Clear out old entities
	for (size_t i = 0; i < entities.size(); i++) {
		delete entities[i];
	}
	entities.clear();

	// Load entities
	for (size_t i = 0; i < map->entities.size(); i++) {
		placeEntity(map->entities[i].type, map->entities[i].x * map->tileSize + map->tileSize / 2, -map->entities[i].y * map->tileSize + map->tileSize / 2);
	}
}

void GameState::CheckForNextLevel() {
	//final version will check if player position is above a blue tile.
	int botX, botY;
	//map->worldToTileCoordinates(player->x_pos, player->y_pos - player->height / 2 - map->tileSize / 2, botX, botY);
	int x = map->getTileCoordinateXPos(player->x_pos);
	if (x == map->mapWidth - 1) { // player reaches end of map
		if (mode == STATE_GAME_LEVEL_1) {
			mode = STATE_GAME_LEVEL_2;
			LoadLevel();
		}
		else if (mode == STATE_GAME_LEVEL_2) {
			mode = STATE_GAME_LEVEL_3;
			LoadLevel();
		}
		else if (mode == STATE_GAME_LEVEL_3) {
			mode = STATE_GAME_OVER;
			LoadLevel();
		}

		resetPlayer();
	}
}