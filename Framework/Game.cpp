// COMP710 GP 2D Framework

// This includes:
#include "Game.h"

// Local includes:
#include "BackBuffer.h"
#include "LogManager.h"
#include "InputHandler.h"
#include "LevelManager.h"
#include "AudioManager.h"
#include "Sprite.h"
#include "Entity.h"
#include "Character.h"
#include "ZombieStates.h"
#include "AmmoBar.h"

// Library includes:
#include <cassert>
#include <SDL.h>

// Static Members:
Game* Game::sp_Instance = 0;

Game&
Game::GetInstance()
{
	if (sp_Instance == 0)
	{
		sp_Instance = new Game();
	}

	assert(sp_Instance);

	return (*sp_Instance);
}

void
Game::DestoryInstance()
{
	delete sp_Instance;
	sp_Instance = 0;
}

Game::Game()
: mp_backBuffer(0)
, mp_inputHandler(0)
, mp_levelManager(0)
, mp_audioManager(0)
, mp_menuManager(0)
, mp_ammoBar(0)
, mp_gameState(0)
, mb_looping(true)
, mf_executionTime(0)
, mf_elapsedSeconds(0)
, mf_lag(0)
, mi_frameCount(0)
, mi_FPS(0)
, mi_numUpdates(0)
, mi_lastTime(0)
, mb_drawDebugInfo(false)
, mi_totalBullets(6)
, mi_currentBullet(0)
//Entities
, mp_player(0)
, mp_car(0)
, mp_gasoline(0)
, mp_distractionAmmo(0)
, gameWorld(nullptr)
{
}


Game::~Game()
{
	// Utilities
	delete mp_inputHandler;
	mp_inputHandler = 0;

	delete mp_backBuffer;
	mp_backBuffer = 0;

	delete mp_levelManager;
	mp_levelManager = 0;

	delete mp_audioManager;
	mp_audioManager = 0;

	delete mp_ammoBar;
	mp_ammoBar = 0;

	delete mp_menuManager;
	mp_menuManager = 0;

	delete mp_gameState;
	mp_gameState = 0;

	// Entities
	delete mp_player;
	mp_player = 0;

	delete mp_car;
	mp_car = 0;

	delete mp_gasoline;
	mp_gasoline = 0;

	for (std::vector<Zombie*>::iterator iter = mp_zombies.begin();
		iter != mp_zombies.end(); ++iter)
	{
		delete *iter;
		*iter = 0;
	}

	for (std::vector<Bullet*>::iterator iter = mp_bullets.begin();
		iter != mp_bullets.end(); ++iter)
	{
		delete *iter;
		*iter = 0;
	}

	for (std::vector<Ammo*>::iterator iter = mp_ammo.begin();
		iter != mp_ammo.end(); ++iter)
	{
		delete *iter;
		*iter = 0;
	}
	
	gameWorld->DestroyBody(gameWorld->GetBodyList());
	delete gameWorld;
	gameWorld = 0;
}

bool
Game::Initialise()
{
	const int width = 800;
	const int height = 600;

	mp_backBuffer = new BackBuffer();
	if (!mp_backBuffer->Initialise(width, height))
	{
		LogManager::GetInstance().Log("BackBuffer Init Fail!");
		return (false);
	}

	mp_backBuffer->SetClearColour(0x00, 0x00, 0x00);

	mp_inputHandler = new InputHandler();
	if (!mp_inputHandler->Initialise())
	{
		LogManager::GetInstance().Log("InputHandler Init Fail!");
		return (false);
	}

	mp_levelManager = new LevelManager();
	if (!mp_levelManager->Initialise(this))
	{
		LogManager::GetInstance().Log("LevelManager Init Fail!");
		return (false);
	}

	mp_audioManager = new AudioManager();
	if (!mp_audioManager->Initialise())
	{
		LogManager::GetInstance().Log("AudioManager Init Fail!");
		return (false);
	}

	mp_menuManager = new MenuManager();
	if (!mp_menuManager->Initialise(mp_backBuffer))
	{
		LogManager::GetInstance().Log("MenuManager Init Fail!");
		return (false);
	}

	mp_ammoBar = new AmmoBar();
	mp_ammoBar->Initialise(mp_backBuffer);

	mp_gameState = new GameState();
	*mp_gameState = START;

	// Music
	mp_audioManager->PlayMusic();

	// Create Box2D world
	const b2Vec2 m_gravity(0.0f, 0.0f);
	gameWorld = new b2World(m_gravity);


	//player
	//Sprite* playerSprite = mp_backBuffer->CreateSprite("assets\\Player\\idle\\survivor-idle_handgun_0.png");
	//Sprite* playerSprite = mp_backBuffer->CreateSprite("assets\\DebuggingAssets\\player_test.png");
	//mp_player = new Player();
	//mp_player->SetPos(b2Vec2(((float)width / 2) - playerSprite->GetWidth() / 2, ((float)height / 2) - playerSprite->GetHeight() / 2));
	//mp_player->Initialise(playerSprite, gameWorld, 0x0002, 0x0001 | 0x0003);


/*
	for (int i = 0; i < mi_totalBullets; ++i)
	{
		Bullet* bullet = new Bullet();
		Sprite* pBulletSprite = mp_backBuffer->CreateSprite("assets\\bullet.png");

		bullet->SetPos(mp_player->GetPos());
		bullet->Initialise(pBulletSprite, gameWorld);

		mp_bullets.push_back(bullet);
	}
*/

	//zombie
	//int zCount = 3;
	//for (int i = 0; i < zCount; i++)
	//{
	//	SpawnZombie(700, 500);
	//}

	////ammo
	//SpawnAmmo(500, 500);

	// Car
	//Sprite* carSprite = mp_backBuffer->CreateSprite("assets\\car1.png");
	//mp_car = new Car();
	////mp_car->SetPositionX(((float)width / 3) - carSprite->GetWidth() / 2);
	////mp_car->SetPositionY(((float)height / 3) - carSprite->GetHeight() / 2);
	//mp_car->SetPos(b2Vec2(((float)width / 3) - carSprite->GetWidth() / 2, ((float)height / 3) - carSprite->GetHeight() / 2));
	//mp_car->Initialise(carSprite, gameWorld);

	// Gasoline
	//Sprite* gasolineSprite = mp_backBuffer->CreateSprite("assets\\gas.png");
	//mp_gasoline = new Gasoline();
	////mp_gasoline->SetPositionX((float)600 - gasolineSprite->GetWidth() / 2);
	////mp_gasoline->SetPositionY((float)200 - gasolineSprite->GetHeight() / 2);
	//mp_gasoline->SetPos(b2Vec2((float)600 - gasolineSprite->GetWidth() / 2, (float)200 - gasolineSprite->GetHeight() / 2));
	//mp_gasoline->Initialise(gasolineSprite, gameWorld);

	// Build level tiles and reposition entities
	LevelBuild();

	mi_lastTime = SDL_GetTicks();
	mf_lag = 0.0f;

	return (true);
}

bool
Game::DoGameLoop()
{
	const float stepSize = 1.0f / 60.0f;
	// Box2D
	const int velocityIterations = 6;
	const int positionIterations = 2;

	assert(mp_inputHandler);
	mp_inputHandler->ProcessInput(*this);

	if (mb_looping)
	{
		int current = SDL_GetTicks();
		float deltaTime = (current - mi_lastTime) / 1000.0f;
		mi_lastTime = current;
		mf_executionTime += deltaTime;
		mf_lag += deltaTime;

		while (mf_lag >= stepSize )
		{
			if (*mp_gameState == PLAYING)
			{
				Process(stepSize);

				mp_audioManager->Update();

				gameWorld->Step(stepSize, velocityIterations, positionIterations);
			}
			else
			{
				//Process(0);

				//gameWorld->Step(0, 0, 0);
				if (mp_menuManager->GetQuit())
					mb_looping = false;
				else if (mp_menuManager->GetRestart())
				{
					//TODO Restart the game
					mp_menuManager->SetRestart(false);
					RestartGame();
					
				}
			}
			mf_lag -= stepSize;
			++mi_numUpdates;
		}

		Draw(*mp_backBuffer);
	}

	SDL_Delay(1);

	return (mb_looping);
}

void
Game::Process(float deltaTime)
{

	// Count total simulation time elapsed:
	mf_elapsedSeconds += deltaTime;

	// Frame Counter:
	if (mf_elapsedSeconds > 1)
	{
		mf_elapsedSeconds -= 1;
		mi_FPS = mi_frameCount;
		mi_frameCount = 0;
	}

	//TODO update
	// Player
	mp_player->Process(deltaTime);

	// Zombie	
	for (std::vector<Zombie*>::iterator iter = mp_zombies.begin();
		iter != mp_zombies.end(); ++iter)
	{
		Zombie* current = *iter;
		current->Process(deltaTime);
		//ZombieDetectPlayer(current);
	}

	if (mp_player->IsDead() && !mp_menuManager->GodMode())
	{
		*mp_gameState = LOST;
		mp_audioManager->PlaySound(se_pDEATH);
	}
		

	// Bullets
	for (std::vector<Bullet*>::iterator iter = mp_bullets.begin();
		iter != mp_bullets.end(); ++iter)
	{
		Bullet* current = *iter;
		current->Process(deltaTime);
	}

	for (std::vector<Ammo*>::iterator iter = mp_ammo.begin();
		iter != mp_ammo.end(); ++iter)
	{
		Ammo* current = *iter;
		current->Process(deltaTime);
	}

	// Car
	mp_car->Process(deltaTime);
	// Gasoline
	if (mp_gasoline != 0)
		mp_gasoline->Process(deltaTime);

	// Level
	LevelMove(); // Adjusts the Players position and Level tiles to allow the full map to scroll on screen
	mp_levelManager->Process(deltaTime);

	int bSize = mp_bullets.size();

	mp_bullets.erase(
		std::remove_if(
		mp_bullets.begin(), mp_bullets.end(),

		[](Bullet* b)
	{
		bool dead = b->IsDead();
		if (dead)
		{
			delete b;
			b = 0;
		}
		return dead;
	}), mp_bullets.end());
	
	int zSize = mp_zombies.size();

	mp_zombies.erase(
		std::remove_if(
		mp_zombies.begin(), mp_zombies.end(),
		[](Zombie* b)
	{
		bool dead = b->IsDead();
		if (dead)
		{
			delete b;
			b = 0;
		}
		return dead;
	}), mp_zombies.end());

	int aSize = mp_ammo.size();
	mp_ammo.erase(
		std::remove_if(
		mp_ammo.begin(), mp_ammo.end(),
		[](Ammo* b)
	{
		bool dead = b->IsDead();
		if (dead)
		{
			delete b;
			b = 0;
		}
		return dead;
	}), mp_ammo.end());

	//If ammo got picked up
	if (mp_ammo.size() < aSize)
		mp_audioManager->PlaySound(se_pPICKUP);

	//If a zombie died
	if (mp_zombies.size() < zSize)
		mp_audioManager->PlaySound(se_zDEATH);

	//If a bullet hit a non-zombie
	if (mp_bullets.size() < bSize && mp_zombies.size() == zSize)
		mp_audioManager->PlaySound(se_pIMPACT);

	if (mp_gasoline != 0 && mp_gasoline->IsDead())
	{
		delete mp_gasoline;
		mp_gasoline = 0;
		mp_audioManager->PlaySound(se_pPICKUP);
	}

	

	//Check if game is won
	for (b2ContactEdge* ce = mp_player->GetBox2DBody()->GetContactList(); ce != NULL; ce = ce->next)
	{

		Entity* tempCar = static_cast<Entity*>(ce->other->GetUserData());
		if (tempCar != NULL && tempCar->GetType() == CAR)
		{
			if (mp_player->HasGasoline())
			{
				*mp_gameState = WON;
				mp_audioManager->PlaySound(se_ENGINE);
			}	
		}
	}
}

void
Game::Draw(BackBuffer& backBuffer)
{
	++mi_frameCount;
	backBuffer.Clear();

	if (*mp_gameState == PLAYING)
	{
		//TODO draw
		mp_levelManager->Draw(backBuffer);
		mp_car->Draw(backBuffer);
		if (mp_gasoline != 0)
			mp_gasoline->Draw(backBuffer);

		for (std::vector<Zombie*>::iterator iter = mp_zombies.begin();
			iter != mp_zombies.end(); ++iter)
		{
			Zombie* current = *iter;
			current->Draw(backBuffer);
		}

		for (std::vector<Bullet*>::iterator iter = mp_bullets.begin();
			iter != mp_bullets.end(); ++iter)
		{
			Bullet* current = *iter;
			current->Draw(backBuffer);
		}

		for (std::vector<Ammo*>::iterator iter = mp_ammo.begin();
			iter != mp_ammo.end(); ++iter)
		{
			Ammo* current = *iter;
			current->Draw(backBuffer);
		}

		mp_player->Draw(backBuffer);

		mp_ammoBar->Draw(mp_backBuffer, mp_player->GetAmmoCount());
	}
	else
		mp_menuManager->DrawMenu(mp_gameState, mp_backBuffer);

	backBuffer.Present();
}

void
Game::Quit()
{
	mb_looping = false;
}

void
Game::VolumeAdjust(bool v)
{
	if (v)
	{
		mp_audioManager->VolumeUp();
	}
	else if (!v)
	{
		mp_audioManager->VolumeDown();
	}
}

void
Game::SpawnPlayer(float x, float y)
{
	mp_player = new Player();
	//Sprite* playerSprite = mp_backBuffer->CreateSprite("assets\\Player\\idle\\survivor-idle_handgun_0.png");
	Sprite* playerSprite = mp_backBuffer->CreateSprite("assets\\DebuggingAssets\\player_test.png");
	
	mp_player->SetPos(b2Vec2(x - playerSprite->GetWidth() / 2, y - playerSprite->GetHeight() / 2));
	mp_player->Initialise(playerSprite, gameWorld, 0x0002, 0x0001 | 0x0003);
}

void
Game::SpawnZombie(float x, float y)
{
	Zombie* tempZombie = new Zombie(mp_audioManager);

	Sprite* pZombieSprite = mp_backBuffer->CreateSprite("assets\\DebuggingAssets\\zombie_test.png");

	tempZombie->SetPos(b2Vec2(x, y));
	tempZombie->Initialise(pZombieSprite, gameWorld, 0x0003, 0x0001 | 0x0002 | 0x0003 | 0x0004 | 0x0005);
	
	//tempZombie->GetSteering()->SetTarget(&GetPlayer()->GetPos());
	tempZombie->GetSteering()->WanderOn();

	mp_zombies.push_back(tempZombie);
}

void
Game::SpawnAmmo(float x, float y)
{
	Ammo* tempAmmo = new Ammo();

	Sprite* pAmmoSprite = mp_backBuffer->CreateSprite("assets\\ammo.png");

	tempAmmo->SetPos(b2Vec2(x, y));
	tempAmmo->Initialise(pAmmoSprite, gameWorld);

	mp_ammo.push_back(tempAmmo);
}

void
Game::SpawnFuel(float x, float y)
{
	mp_gasoline = new Gasoline();

	Sprite* gasolineSprite = mp_backBuffer->CreateSprite("assets\\gas.png");

	mp_gasoline->SetPos(b2Vec2(x - gasolineSprite->GetWidth() / 2, y - gasolineSprite->GetHeight() / 2));
	mp_gasoline->Initialise(gasolineSprite, gameWorld);
}

void
Game::SpawnCar(float x, float y)
{
	mp_car = new Car();

	Sprite* carSprite = mp_backBuffer->CreateSprite("assets\\car1.png");
	
	mp_car->SetPos(b2Vec2(x - carSprite->GetWidth() / 2, y - carSprite->GetHeight() / 2));
	mp_car->Initialise(carSprite, gameWorld);
}

void
Game::LevelBuild()
{
	mp_levelManager->CreateLevel(mp_backBuffer, gameWorld);
}

// Moves tiles when Player leaves the central bounds of the playable space. Allowing the level to scroll.
void
Game::LevelMove()
{
	const float speedTiles = 1.0f;
	const float distanceCorrection = 0.1f;
	const float distanceCorrectionXY = 0.165f;
	// Box2D scaling values used. * by 0.1 
	const int leftBoundX = 20;
	const int rightBoundX = 60;
	const int upperBoundY = 20;
	const int lowerBoundY = 40;

	b2Body* tempBody = mp_player->GetBox2DBody();
	float tempPosX = tempBody->GetPosition().x;
	float tempPosY = tempBody->GetPosition().y;

	// Adjust objects to counter act players movement
	// Reposition player and shift the level instead when the player moves out of the "moveable region"
	if (tempPosX < leftBoundX && tempPosY < upperBoundY)
	{
		LevelMoveChangeTransform(distanceCorrectionXY, distanceCorrectionXY);

		tempBody->SetTransform(b2Vec2(leftBoundX + distanceCorrection, upperBoundY + distanceCorrection), tempBody->GetAngle());
	}
	else if (tempPosY < upperBoundY && tempPosX > rightBoundX)
	{
		LevelMoveChangeTransform(-distanceCorrectionXY, distanceCorrectionXY);

		tempBody->SetTransform(b2Vec2(rightBoundX - distanceCorrection, upperBoundY + distanceCorrection), tempBody->GetAngle());
	}
	else if (tempPosX < leftBoundX && tempPosY > lowerBoundY)
	{
		LevelMoveChangeTransform(distanceCorrectionXY, -distanceCorrectionXY);

		tempBody->SetTransform(b2Vec2(leftBoundX + distanceCorrection, lowerBoundY - distanceCorrection), tempBody->GetAngle());
	}
	else if (tempPosY > lowerBoundY && tempPosX > rightBoundX)
	{
		LevelMoveChangeTransform(-distanceCorrectionXY, -distanceCorrectionXY);

		tempBody->SetTransform(b2Vec2(rightBoundX - distanceCorrection, lowerBoundY - distanceCorrection), tempBody->GetAngle());
	}

	// X axis
	else if (tempPosX < leftBoundX)
	{
		LevelMoveChangeTransform(distanceCorrectionXY, 0);

		tempBody->SetTransform(b2Vec2(leftBoundX + distanceCorrection, tempPosY), tempBody->GetAngle());
	}
	else if (tempPosX > rightBoundX)
	{
		LevelMoveChangeTransform(-distanceCorrectionXY, 0);

		tempBody->SetTransform(b2Vec2(rightBoundX - distanceCorrection, tempPosY), tempBody->GetAngle());
	}
	// Y axis
	else if (tempPosY < upperBoundY)
	{
		LevelMoveChangeTransform(0, distanceCorrectionXY);

		tempBody->SetTransform(b2Vec2(tempPosX, upperBoundY + distanceCorrection), tempBody->GetAngle());
	}
	else if (tempPosY > lowerBoundY)
	{
		LevelMoveChangeTransform(0, -distanceCorrectionXY);

		tempBody->SetTransform(b2Vec2(tempPosX, lowerBoundY - distanceCorrection), tempBody->GetAngle());
	}
}

void
Game::LevelMoveChangeTransform(float x, float y)
{
	
	mp_levelManager->LevelMoveTiles(x, y);
	mp_car->Move(x, y);
	if (mp_gasoline != 0)
	{
		mp_gasoline->Move(x, y);
	}

	for (int i = 0; i < mp_bullets.size(); ++i)
	{
		b2Body* tempBody = mp_bullets[i]->GetBox2DBody();
		if (tempBody->GetPosition().x + mp_bullets[i]->GetOffsetX() < 0 - mp_bullets[i]->GetWidth() * 4 || tempBody->GetPosition().x + mp_bullets[i]->GetOffsetX() > 80 + mp_bullets[i]->GetWidth() * 4 ||
			tempBody->GetPosition().y + mp_bullets[i]->GetOffsetY() < 0 - mp_bullets[i]->GetHeight() * 4 || tempBody->GetPosition().y + mp_bullets[i]->GetOffsetY() > 60 + mp_bullets[i]->GetHeight() * 4)
		{
			mp_bullets[i]->SetOffsetX(x);
			mp_bullets[i]->SetOffsetY(y);
		}

		if (tempBody->GetPosition().x + mp_bullets[i]->GetOffsetX() >= 0 - mp_bullets[i]->GetWidth() * 4 && tempBody->GetPosition().x + mp_bullets[i]->GetOffsetX() <= 80 + mp_bullets[i]->GetWidth() * 4 &&
			tempBody->GetPosition().y + mp_bullets[i]->GetOffsetY() >= 0 - mp_bullets[i]->GetHeight() * 4 && tempBody->GetPosition().y + mp_bullets[i]->GetOffsetY() <= 60 + mp_bullets[i]->GetHeight() * 4)
		{
			tempBody->SetTransform(b2Vec2(tempBody->GetPosition().x + x + mp_bullets[i]->GetOffsetX(), tempBody->GetPosition().y + y + mp_bullets[i]->GetOffsetY()), tempBody->GetAngle());
			mp_bullets[i]->SetOffsetX(0);
			mp_bullets[i]->SetOffsetY(0);
		}
	}
	for (int i = 0; i < mp_zombies.size(); ++i)
	{
		b2Body* tempBody = mp_zombies[i]->GetBox2DBody();
		if (tempBody->GetPosition().x + mp_zombies[i]->GetOffsetX() < 0 - mp_zombies[i]->GetWidth() * 4 || tempBody->GetPosition().x + mp_zombies[i]->GetOffsetX() > 80 + mp_zombies[i]->GetWidth() * 4 ||
			tempBody->GetPosition().y + mp_zombies[i]->GetOffsetY() < 0 - mp_zombies[i]->GetHeight() * 4 || tempBody->GetPosition().y + mp_zombies[i]->GetOffsetY() > 60 + mp_zombies[i]->GetHeight() * 4)
		{
			mp_zombies[i]->SetOffsetX(x);
			mp_zombies[i]->SetOffsetY(y);
		}

		if (tempBody->GetPosition().x + mp_zombies[i]->GetOffsetX() >= 0 - mp_zombies[i]->GetWidth() * 4 && tempBody->GetPosition().x + mp_zombies[i]->GetOffsetX() <= 80 + mp_zombies[i]->GetWidth() * 4 &&
			tempBody->GetPosition().y + mp_zombies[i]->GetOffsetY() >= 0 - mp_zombies[i]->GetHeight() * 4 && tempBody->GetPosition().y + mp_zombies[i]->GetOffsetY() <= 60 + mp_zombies[i]->GetHeight() * 4)
		{
			tempBody->SetTransform(b2Vec2(tempBody->GetPosition().x + x + mp_zombies[i]->GetOffsetX(), tempBody->GetPosition().y + y + mp_zombies[i]->GetOffsetY()), tempBody->GetAngle());
			mp_zombies[i]->SetOffsetX(0);
			mp_zombies[i]->SetOffsetY(0);
		}
	}
	for (int i = 0; i < mp_ammo.size(); ++i)
	{
		b2Body* tempBody = mp_ammo[i]->GetBox2DBody();
		if (tempBody->GetPosition().x + mp_ammo[i]->GetOffsetX() < 0 - mp_ammo[i]->GetWidth() * 4 || tempBody->GetPosition().x + mp_ammo[i]->GetOffsetX() > 80 + mp_ammo[i]->GetWidth() * 4 ||
			tempBody->GetPosition().y + mp_ammo[i]->GetOffsetY() < 0 - mp_ammo[i]->GetHeight() * 4 || tempBody->GetPosition().y + mp_ammo[i]->GetOffsetY() > 60 + mp_ammo[i]->GetHeight() * 4)
		{
			mp_ammo[i]->SetOffsetX(x);
			mp_ammo[i]->SetOffsetY(y);
		}

		if (tempBody->GetPosition().x + mp_ammo[i]->GetOffsetX() >= 0 - mp_ammo[i]->GetWidth() * 4 && tempBody->GetPosition().x + mp_ammo[i]->GetOffsetX() <= 80 + mp_ammo[i]->GetWidth() * 4 &&
			tempBody->GetPosition().y + mp_ammo[i]->GetOffsetY() >= 0 - mp_ammo[i]->GetHeight() * 4 && tempBody->GetPosition().y + mp_ammo[i]->GetOffsetY() <= 60 + mp_ammo[i]->GetHeight() * 4)
		{
			tempBody->SetTransform(b2Vec2(tempBody->GetPosition().x + x + mp_ammo[i]->GetOffsetX(), tempBody->GetPosition().y + y + mp_ammo[i]->GetOffsetY()), tempBody->GetAngle());
			mp_ammo[i]->SetOffsetX(0);
			mp_ammo[i]->SetOffsetY(0);
		}
	}
}

Player*
Game::GetPlayer()
{
	return mp_player;
}


void
Game::ShootGun()
{
	if (mp_player->GetAmmoCount() > 0)
	{
		if (!mp_menuManager->InfiniteAmmo())
			mp_player->DecreaseAmmo();

		Bullet* tempBullet = new Bullet();

		Sprite* pBulletSprite = mp_backBuffer->CreateSprite("assets\\bullet.png");

		tempBullet->SetPos(mp_player->GetPos());
		tempBullet->Initialise(pBulletSprite, gameWorld);

		tempBullet->Rotate(mp_player->GetBox2DBody()->GetAngle(), mp_player->GetWidth()/2, mp_player->GetHeight()/2);
		tempBullet->Move(mp_player->GetHeading().x, mp_player->GetHeading().y);

		mp_bullets.push_back(tempBullet);
		mp_audioManager->PlaySound(se_pSHOOT);
	}
	else
	{
		//TODO Play empty gun noise
		mp_audioManager->PlaySound(se_pRELOAD);
	}
}

AudioManager*
Game::GetAudioManager()
{
	return mp_audioManager;
}

MenuManager*
Game::GetMenuManager()
{
	return mp_menuManager;
}

GameState*
Game::GetGameState()
{
	return mp_gameState;

}

void
Game::PauseGame()
{
	*mp_gameState = PAUSED;
}

void
Game::RestartGame()
{
	*mp_gameState = START;


	for (std::vector<Zombie*>::iterator iter = mp_zombies.begin();
		iter != mp_zombies.end(); ++iter)
	{
		delete *iter;
		*iter = 0;
	}

	mp_zombies.clear();

	for (std::vector<Bullet*>::iterator iter = mp_bullets.begin();
		iter != mp_bullets.end(); ++iter)
	{
		delete *iter;
		*iter = 0;
	}

	mp_bullets.clear();

	for (std::vector<Ammo*>::iterator iter = mp_ammo.begin();
		iter != mp_ammo.end(); ++iter)
	{
		delete *iter;
		*iter = 0;
	}

	mp_ammo.clear();

	delete mp_car;
	mp_car = 0;

	delete mp_player;
	mp_player = 0;

	if (mp_gasoline != 0)
	{
		delete mp_gasoline;
		mp_gasoline = 0;
	}

	delete mp_levelManager;
	mp_levelManager = 0;

	gameWorld->DestroyBody(gameWorld->GetBodyList());
	delete gameWorld;
	gameWorld = 0;

	mp_levelManager = new LevelManager();
	if (!mp_levelManager->Initialise(this))
	{
		LogManager::GetInstance().Log("LevelManager Init Fail!");
		//return (false);
	}

	
	LevelBuild();

	

	// Car
	Sprite* carSprite = mp_backBuffer->CreateSprite("assets\\car1.png");
	mp_car = new Car();
	//mp_car->SetPositionX(((float)width / 3) - carSprite->GetWidth() / 2);
	//mp_car->SetPositionY(((float)height / 3) - carSprite->GetHeight() / 2);
	mp_car->SetPos(b2Vec2(((float)800 / 3) - carSprite->GetWidth() / 2, ((float)600 / 3) - carSprite->GetHeight() / 2));
	mp_car->Initialise(carSprite, gameWorld);

	

	//player
	//Sprite* playerSprite = mp_backBuffer->CreateSprite("assets\\Player\\idle\\survivor-idle_handgun_0.png");
	Sprite* playerSprite = mp_backBuffer->CreateSprite("assets\\DebuggingAssets\\player_test.png");
	mp_player = new Player();
	//mp_player->SetPositionX(((float)width / 2) - playerSprite->GetWidth() / 2);
	//mp_player->SetPositionY(((float)height / 2) - playerSprite->GetHeight() / 2);
	mp_player->SetPos(b2Vec2(((float)800 / 2) - playerSprite->GetWidth() / 2, ((float)600 / 2) - playerSprite->GetHeight() / 2));
	mp_player->Initialise(playerSprite, gameWorld, 0x0002, 0x0001 | 0x0003);

	//zombie
	int zCount = 3;
	for (int i = 0; i < zCount; i++)
	{
		SpawnZombie(100, 100);
	}

	//ammo
	SpawnAmmo(300, 300);

	// Gasoline
	Sprite* gasolineSprite = mp_backBuffer->CreateSprite("assets\\gas.png");
	mp_gasoline = new Gasoline();
	//mp_gasoline->SetPositionX((float)600 - gasolineSprite->GetWidth() / 2);
	//mp_gasoline->SetPositionY((float)200 - gasolineSprite->GetHeight() / 2);
	mp_gasoline->SetPos(b2Vec2((float)600 - gasolineSprite->GetWidth() / 2, (float)200 - gasolineSprite->GetHeight() / 2));
	mp_gasoline->Initialise(gasolineSprite, gameWorld);
}