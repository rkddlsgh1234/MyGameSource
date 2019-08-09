#include "Enemy.h"



Enemy::Enemy()
{
	m_iAttackDelay = 0;

	m_fDelay = 0;
	m_iRandomDelay = 250;
}


Enemy::~Enemy()
{
}


int Enemy::MoveTo(float ElapseTime)
{
	m_fDelay = m_fDelay + 70 * ElapseTime;

	if (m_fDelay > m_iRandomDelay)
	{
		m_fDelay = 0;
		while (true)
		{
			TANK_MOVE_STATE RandomWay;
			int Random = rand() % 4;

			if (Random == 0)
				RandomWay = TANK_STATE_DOWN_00;
			else if (Random == 1)
				RandomWay = TANK_STATE_UP_00;
			else if (Random == 2)
				RandomWay = TANK_STATE_RIGHT_00;
			else if (Random == 3)
				RandomWay = TANK_STATE_LEFT_00;

			if (RandomWay != Tank::GetState())
			{
				Tank::TurnWay(RandomWay);
				m_iRandomDelay = rand() % 300 + 100;
				return 0;
			}
		}
	}
	int iState = Tank::GetState();
	int iKeyType;

	if (iState == TANK_STATE_LEFT_00 || iState == TANK_STATE_LEFT_01)
		iKeyType = VK_LEFT;
	else if (iState == TANK_STATE_RIGHT_00 || iState == TANK_STATE_RIGHT_01)
		iKeyType = VK_RIGHT;
	else if (iState == TANK_STATE_UP_00 || iState == TANK_STATE_UP_01)
		iKeyType = VK_UP;
	else if (iState == TANK_STATE_DOWN_00 || iState == TANK_STATE_DOWN_01)
		iKeyType = VK_DOWN;

	Tank::Move(ElapseTime, iKeyType);
}

void Enemy::AttackTo(float ElapseTime)
{
	m_iAttackDelay = m_iAttackDelay + 70 * ElapseTime;

	if (m_iAttackDelay > 50)
	{
		m_iAttackDelay = 0;
		Tank::Attack();
	}
}