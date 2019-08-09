#include "Enemy.h"
#include "BitMap.h"
#include "Player.h"

Enemy::Enemy()
{
}


Enemy::~Enemy()
{
}

void Enemy::Init(BitMap * pBitMap00, BitMap * pBitMap01, int x, int y, int Num)
{
	m_iX = (float)x;
	m_iY = (float)y + 70.0;
	m_iDelay = 0;
	m_bCheckScore = false;
	m_bSameScore = false;
	m_bStop = false;
	m_iEnemyState = ENEMY_STATE_RUN_00;
	m_iNum = Num;
	m_pBitMap00 = pBitMap00;
	m_pBitMap01 = pBitMap01;
}

HDC Enemy::Draw()
{
	if (m_iEnemyState == ENEMY_STATE_RUN_00)
		return m_pBitMap00->Draw();
	else if (m_iEnemyState == ENEMY_STATE_RUN_01)
		return m_pBitMap01->Draw();

	return m_pBitMap01->Draw();
}
HBITMAP Enemy::ReturnHbitmap()
{
	if(m_iEnemyState == ENEMY_STATE_RUN_00)
		return m_pBitMap00->ReturnHbitmap();
	else if (m_iEnemyState == ENEMY_STATE_RUN_01)
		return m_pBitMap01->ReturnHbitmap();

	return m_pBitMap01->ReturnHbitmap();
}

void Enemy::Move(float Distance, int Type)
{
	if (m_bStop)
		return;

	if (Type == 0)
		m_iX += Distance;
	else if (Type == 1)
		m_iX -= Distance;
}

void Enemy::MoveMotion() // 애니메이션 모션
{
	if (m_bStop)
		return;

	m_iDelay++;

	if (m_iDelay > 10)
	{
		m_iEnemyState++;

		if (m_iEnemyState == 2)
			m_iEnemyState = ENEMY_STATE_RUN_00;

		m_iDelay = 0;
	}
}

void Enemy::MoveScroll(int iState)
{
}

void Enemy::PlusScore() // 장애물 패스 체크
{
	if (m_bSameScore)
		return;

	m_bCheckScore = true;
	m_bSameScore = true;
}
void Enemy::DeleteScore()
{
	m_bCheckScore = false;
}

void Enemy::StopEnemy() // 장애물 동작 정지
{
	m_bStop = true;
	m_iX = 500;
	m_iY = 500;
}

void Enemy::Reset(float x)
{
	m_bCheckScore = false;
	m_bStop = false;
	m_bSameScore = false;
	if(x != 0)
		m_iX = x;
	m_iY = 203;
}
void Enemy::MoveSet()
{
	m_bStop = false;
}