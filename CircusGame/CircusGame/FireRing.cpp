#include "FireRing.h"
#include "Player.h"
#include "GameManager.h"
#include "Player.h"

FireRing::FireRing()
{
}


FireRing::~FireRing()
{
}

void FireRing::MoveScroll(int iState)
{
	if (GameManager::GetInstance()->GetEndLine())
	{
		Enemy::Move((float)1.2, 1);
		return;
	}

	if (iState == VK_LEFT || iState == PLAYER_STATE_LEFTJUMP)
		Enemy::Move((float)1.4, 0);
	else if (iState == VK_RIGHT || iState == PLAYER_STATE_RIGHTJUMP)
		Enemy::Move((float)2.8, 1);
	else
		Enemy::Move((float)1.4, 1);
}