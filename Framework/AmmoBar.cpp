#include "AmmoBar.h"

AmmoBar::AmmoBar()
	: sp_bullet1(nullptr)
	, sp_bullet2(nullptr)
	, sp_bullet3(nullptr)
	, sp_bullet4(nullptr)
	, sp_bullet5(nullptr)
	, sp_bullet6(nullptr)
{
}

AmmoBar::~AmmoBar()
{
	delete sp_bullet1;
	sp_bullet1 = 0;
	delete sp_bullet2;
	sp_bullet2 = 0;
	delete sp_bullet3;
	sp_bullet3 = 0;

	delete sp_bullet4;
	sp_bullet4 = 0;
	delete sp_bullet5;
	sp_bullet5 = 0;
	delete sp_bullet6;
	sp_bullet6 = 0;
}

bool
AmmoBar::Initialise(BackBuffer* backBuffer)
{
	sp_bullet1 = backBuffer->CreateSprite("assets\\bullet.png");
	sp_bullet2 = backBuffer->CreateSprite("assets\\bullet.png");
	sp_bullet3 = backBuffer->CreateSprite("assets\\bullet.png");

	sp_bullet4 = backBuffer->CreateSprite("assets\\bullet.png");
	sp_bullet5 = backBuffer->CreateSprite("assets\\bullet.png");
	sp_bullet6 = backBuffer->CreateSprite("assets\\bullet.png");

	sp_bullet1->SetPos(b2Vec2(30, 550));
	sp_bullet1->SetAngle(270);
	sp_bullet2->SetPos(b2Vec2(43, 550));
	sp_bullet2->SetAngle(270);
	sp_bullet3->SetPos(b2Vec2(56, 550));
	sp_bullet3->SetAngle(270);
	sp_bullet4->SetPos(b2Vec2(69, 550));
	sp_bullet4->SetAngle(270);
	sp_bullet5->SetPos(b2Vec2(82, 550));
	sp_bullet5->SetAngle(270);
	sp_bullet6->SetPos(b2Vec2(95, 550));
	sp_bullet6->SetAngle(270);

	return (true);
}

void
AmmoBar::Draw(BackBuffer* backBuffer, int n)
{
	if (n > 0)
		backBuffer->DrawSprite(*sp_bullet1);
	if (n > 1)
		backBuffer->DrawSprite(*sp_bullet2);
	if (n > 2)
		backBuffer->DrawSprite(*sp_bullet3);
	if (n > 3)
		backBuffer->DrawSprite(*sp_bullet4);
	if (n > 4)
		backBuffer->DrawSprite(*sp_bullet5);
	if (n > 5)
		backBuffer->DrawSprite(*sp_bullet6);
}

