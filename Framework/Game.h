#ifndef __GAME_H__
#define __GAME_H__
#include "BackBuffer.h"
#include "InputHandler.h"
#include "Zombie.h"
#include "Player.h"
#include "Car.h"
#include "Gasoline.h"
#include "Distraction.h"
#include "MenuManager.h"
#include "GameStates.h"

// Library includes:
#include <Box2D.h>

// Forward declarations
class LevelManager;
class AudioManager;
class AmmoBar;

class Game
{
	// Member Methods:
public:
	static Game& GetInstance();
	static void DestoryInstance();
	~Game();

	bool Initialise();
	bool DoGameLoop();
	void Quit();

	void SpawnPlayer(float x, float y);
	void SpawnZombie(float x, float y);
	void SpawnAmmo(float x, float y);
	void SpawnFuel(float x, float y);
	void SpawnCar(float x, float y);

	void VolumeAdjust(bool v);

	Player* GetPlayer();

	void ShootGun();

	AudioManager* GetAudioManager();
	MenuManager* GetMenuManager();
	AmmoBar* GetAmmoBar();
	GameState* GetGameState();

	void PauseGame();
	void RestartGame();

protected:
	void Process(float deltaTime);
	void Draw(BackBuffer& backBuffer);

	void LevelBuild();
	void LevelMove();
	void LevelMoveChangeTransform(float x, float y);
	
	//void SpawnDistractionAmmo();

private:
	Game(const Game& game);
	Game& operator=(const Game& game);

	Game();

	// Member Data:
public:
		
protected:
	static Game* sp_Instance;
	BackBuffer* mp_backBuffer;
	InputHandler* mp_inputHandler;
	LevelManager* mp_levelManager;
	AudioManager* mp_audioManager;
	MenuManager* mp_menuManager;
	AmmoBar* mp_ammoBar;
	GameState* mp_gameState;

	bool mb_looping;

	float mf_executionTime;
	float mf_elapsedSeconds;
	float mf_lag;
	int mi_frameCount;
	int mi_FPS;
	int mi_numUpdates;
	int mi_lastTime;
	bool mb_drawDebugInfo;

	int mi_totalBullets;
	int mi_currentBullet;

	Player* mp_player;
	Car* mp_car;
	Gasoline* mp_gasoline;

	std::vector<Distraction*> mp_distractionAmmo;
	std::vector<Zombie*> mp_zombies;
	std::vector<Bullet*> mp_bullets;
	std::vector<Ammo*> mp_ammo;


	//Box2D
	b2World* gameWorld;
private:

};

#endif //__GAME_H__