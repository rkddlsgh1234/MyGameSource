#pragma once
#include "Tank.h"

class Enemy : public Tank
{
private:
	int m_iAttackDelay;
	int m_iRandomDelay;
	float m_fDelay;
	float m_fWayDelay;

public:
	Enemy();
	~Enemy();

	virtual int MoveTo(float ElapseTime);
	virtual void AttackTo(float ElapseTime);
};

