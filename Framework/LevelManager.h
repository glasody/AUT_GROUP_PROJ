#ifndef __LEVELMANAGER_H__
#define __LEVELMANAGER_H__

// Library includes
#include <fstream>
#include <Box2D.h>
#include <vector>

// Local includes
#include "Game.h"

// Forward declarations
class BackBuffer;
class Sprite;
class Tile;

typedef enum
{
	GROUND_T = 'G',
	WALL_T = 'W',
	BUILDING_T = 'B',
	PLAYER_T = 'P',
	ZOMBIE_OUT_T = 'Z',
	ZOMBIE_IN_T = 'J',
	AMMO_T = 'A',
	FUEL_T = 'F',
	CAR_T = 'C',
	DISTRACTION_T = 'D'
}tileTypes;

class LevelManager
{
	// Member methods
public:
	LevelManager();
	~LevelManager();

	bool Initialise(Game* game);
	void Process(float deltaTime);
	void Draw(BackBuffer& backBuffer);

	void CreateLevel(BackBuffer* mp_backBuffer, b2World* gameworld);
	void SetupTile(Sprite* sprite, int posW, int posH);
	void SetupCollisionTile(Sprite* sprite, int posW, int posH, b2World* gameWorld, bool collision);
	void LevelMoveTiles(float x, float y);
	void LevelStopTiles();

	void ReadFile();

	std::vector<Tile*> GetTiles();

protected:

private:

	// Member data
public:
protected:
	int mi_levelWidth;  // Number of tiles
	int mi_levelHeight; // Number of tiles

	std::ifstream mapFile;

	std::vector<Tile*> tiles;

	Game* mp_game;

private:

};

#endif // __LEVELMANAGER_H__