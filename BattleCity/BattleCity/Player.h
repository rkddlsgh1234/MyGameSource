#pragma once
#include "Tank.h"

class Player : public Tank
{
public:
	Player();
	~Player();

	virtual int MoveTo(float fElapseTime);
};

