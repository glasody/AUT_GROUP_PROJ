
// This includes:
#include "LevelManager.h"

// Local includes:
#include "BackBuffer.h"
#include "Sprite.h"
#include "Tile.h"
#include <string>

// Library includes:

LevelManager::LevelManager()
: mi_levelWidth(0)
, mi_levelHeight(0)
, mp_game(0)
{

}

LevelManager::~LevelManager()
{
	// delete tiles
	for (unsigned int i = 0; i < tiles.size(); ++i)
	{
		//tiles[i]->~Tile();
		delete tiles[i];
		tiles[i] = 0;
	}
	tiles.clear();
	//std::vector<Tile*>().swap(tiles);
}

bool
LevelManager::Initialise(Game* game)
{
	mp_game = game;

	ReadFile();

	return (true);
}

void
LevelManager::Process(float deltaTime)
{
	for (unsigned int i = 0; i < tiles.size(); ++i)
	{
		tiles[i]->Process(deltaTime);
	}
}

void
LevelManager::Draw(BackBuffer& backBuffer)
{
	for (unsigned int i = 0; i < tiles.size(); ++i)
	{
		tiles[i]->Draw(backBuffer);
	}
}

void
LevelManager::ReadFile()
{
	mapFile.open("assets\\Level\\maplayout.txt");
}

void
LevelManager::CreateLevel(BackBuffer* mp_backBuffer, b2World* gameWorld)
{
	char tileID;
	
	// Find Width and Height specified in file. Convert to int.
	std::string strWidth;
	std::getline(mapFile, strWidth);
	std::string strHeight;
	std::getline(mapFile, strHeight);
	int mi_levelWidth = atoi(strWidth.c_str());
	int mi_levelHeight = atoi(strHeight.c_str());

	// Iterate through .txt file and setup tiles.
	for (int posH = 0; posH < mi_levelHeight; ++posH)
	{
		for (int posW = 0; posW < mi_levelWidth; ++posW)
		{
			Sprite* pTileSprite;
			
			mapFile >> (tileID);

			switch (tileID)
			{
			case GROUND_T:
				//pTileSprite = mp_backBuffer->CreateSprite("assets\\ground.png");
				pTileSprite = mp_backBuffer->CreateSprite("assets\\Level\\ground.png");

				SetupCollisionTile(pTileSprite, posW, posH, gameWorld, false);
				break;
			case WALL_T:
				//pTileSprite = mp_backBuffer->CreateSprite("assets\\wall.png");
				pTileSprite = mp_backBuffer->CreateSprite("assets\\Level\\wall.png");
					
				SetupCollisionTile(pTileSprite, posW, posH, gameWorld, true);
				break;
			case BUILDING_T:
				pTileSprite = mp_backBuffer->CreateSprite("assets\\Level\\building.png");

				SetupCollisionTile(pTileSprite, posW, posH, gameWorld, false);
				break;
/////////////////////////////////////////////////////////
			case PLAYER_T:
				pTileSprite = mp_backBuffer->CreateSprite("assets\\Level\\building.png");

				SetupCollisionTile(pTileSprite, posW, posH, gameWorld, false);

				// Add player
				mp_game->SpawnPlayer(posW, posH);
				break;
			case ZOMBIE_OUT_T:
				pTileSprite = mp_backBuffer->CreateSprite("assets\\Level\\ground.png");

				SetupCollisionTile(pTileSprite, posW, posH, gameWorld, false);

				// Add a zombie
				mp_game->SpawnZombie(posW, posH);
				break;
			case ZOMBIE_IN_T:
				pTileSprite = mp_backBuffer->CreateSprite("assets\\Level\\building.png");

				SetupCollisionTile(pTileSprite, posW, posH, gameWorld, false);

				// Add a zombie
				mp_game->SpawnZombie(posW, posH);
				break;
			case AMMO_T:
				pTileSprite = mp_backBuffer->CreateSprite("assets\\Level\\building.png");

				SetupCollisionTile(pTileSprite, posW, posH, gameWorld, false);

				// Add an ammo pickup
				mp_game->SpawnAmmo(posW, posH);
				break;
			case FUEL_T:
				pTileSprite = mp_backBuffer->CreateSprite("assets\\Level\\building.png");

				SetupCollisionTile(pTileSprite, posW, posH, gameWorld, false);

				// Add a fuel pickup
				mp_game->SpawnFuel(posW, posH);
				break;
			case CAR_T:
				pTileSprite = mp_backBuffer->CreateSprite("assets\\Level\\ground.png");

				SetupCollisionTile(pTileSprite, posW, posH, gameWorld, false);

				// Add a car
				mp_game->SpawnCar(posW, posH);
				break;
			case DISTRACTION_T:
				pTileSprite = mp_backBuffer->CreateSprite("assets\\Level\\building.png");

				SetupCollisionTile(pTileSprite, posW, posH, gameWorld, false);
				break;
/////////////////////////////////////////////////////////
			default:
				// Used to catch cases where the tileID does not match. Will draw placeholder asset.
				pTileSprite = mp_backBuffer->CreateSprite("assets\\DebuggingAssets\\placeholder.png");

				SetupCollisionTile(pTileSprite, posW, posH, gameWorld, false);
				break;
			}
		}
	}
}

// Creates a Tile WITHOUT Box2D properties
void 
LevelManager::SetupTile(Sprite* sprite, int posW, int posH)
{
	// Get Sprite width / height
	int sW = sprite->GetWidth();
	int sH = sprite->GetWidth();

	Tile* tile = new Tile();

	tile->SetPos(b2Vec2((float)posW * sW, (float)posH * sH));
	tile->Initialise(sprite); // Height and Width taken as half lengths object width = 2*given width

	tiles.push_back(tile);
}

// Creates a tile WITH Box2D properties
void
LevelManager::SetupCollisionTile(Sprite* sprite, int posW, int posH, b2World* gameWorld, bool collision)
{
	// Get Sprite width / height
	int sW = sprite->GetWidth();
	int sH = sprite->GetWidth();

	Tile* tile = new Tile();

	tile->SetPos(b2Vec2((float)posW * sW, (float)posH * sH));
	tile->Initialise(sprite, gameWorld, collision); 

	tiles.push_back(tile);
}

void
LevelManager::LevelMoveTiles(float x, float y)
{
	for (unsigned int i = 0; i < tiles.size(); ++i)
	{
		if (tiles[i]->GetBox2DBody() != 0)
		{
			tiles[i]->Move(x, y);
		}
	}
}

void
LevelManager::LevelStopTiles()
{
	for (unsigned int i = 0; i < tiles.size(); ++i)
	{
		if (tiles[i]->GetBox2DBody() != 0)
		{
			tiles[i]->GetBox2DBody()->SetLinearVelocity(b2Vec2(0, 0));
		}
	}
}

std::vector<Tile*>
LevelManager::GetTiles()
{
	return tiles;
}