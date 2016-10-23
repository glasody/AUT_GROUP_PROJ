#ifndef __AmmoBar_H__
#define __AmmoBar_H__
#include "Sprite.h"
#include "BackBuffer.h"

class AmmoBar
{
	//Member Methods

public:
	AmmoBar();
	~AmmoBar();

	bool Initialise(BackBuffer* backBuffer);

	void Draw(BackBuffer* backBuffer, int n);

protected:

private:

	//Member Data
public:

protected:

	Sprite* sp_bullet1;
	Sprite* sp_bullet2;
	Sprite* sp_bullet3;
	Sprite* sp_bullet4;
	Sprite* sp_bullet5;
	Sprite* sp_bullet6;


private:

};

#endif //__AmmoBar_H__