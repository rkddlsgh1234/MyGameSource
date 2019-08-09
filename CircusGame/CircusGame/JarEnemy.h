#pragma once
#include "Enemy.h"

#define FRONT 50

class BitMap;
class JarEnemy : public Enemy
{
public:
	JarEnemy();
	~JarEnemy();

	virtual void MoveScroll(int iState);

	virtual inline POINT GetSize()
	{
		POINT Size = { FRONT, FRONT };
		return Size;
	}
};

