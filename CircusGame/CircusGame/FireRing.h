#pragma once
#include "Enemy.h"

#define ENEMY_X 25
#define ENEMY_Y 130

class BitMap;
class FireRing : public Enemy
{
public:
	FireRing();
	~FireRing();

	virtual void MoveScroll(int iState);

	virtual inline POINT GetSize()
	{
		POINT Size = { ENEMY_X, ENEMY_Y };
		return Size;
	}
};

