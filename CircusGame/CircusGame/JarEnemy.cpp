#include "JarEnemy.h"
#include "Player.h"
#include "GameManager.h"

JarEnemy::JarEnemy()
{
}


JarEnemy::~JarEnemy()
{
}

void JarEnemy::MoveScroll(int iState)
{	
	if (GameManager::GetInstance()->GetEndLine())
		return;

	if (iState == NULL || iState == PLAYER_STATE_JUMP)
		return;
	else if (iState == VK_LEFT || iState == PLAYER_STATE_LEFTJUMP)
		Enemy::Move(2.0, 0);
	else if (iState == VK_RIGHT || iState == PLAYER_STATE_RIGHTJUMP)
		Enemy::Move(2.0, 1);
}

