#ifndef __AMMO_H__
#define __AMMO_H__
#include "Item.h"
class Ammo :
	public Entity
{
	// Member Methods:
public:
	Ammo();
	~Ammo();

	bool Initialise(Sprite* sprite, b2World* gameWorld);

	void Process(float deltaTime);
	void Draw(BackBuffer& backBuffer);

	b2Body* GetBox2DBody();
	void Move(float x, float y);
	void StopMove();

	int GetQuantity();
	void SetQuantity(int quantity);
protected:

private:
	void box2DBodyCreate(b2World* gameWorld, float width, float height);
	// Member Data:
public:

protected:

	int mi_quantity;
private:
	b2Body* mp_body;
};

#endif //__AMMO_H__