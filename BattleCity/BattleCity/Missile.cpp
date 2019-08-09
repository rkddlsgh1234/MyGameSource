#include "Missile.h"
#include "BitMap.h"
#include "Tank.h"

Missile::Missile()
{
}


Missile::~Missile()
{
}

void Missile::Init(float x, float y, MISSILE_WAY Way, int iTeam)
{
	m_iX = (float)x;
	m_iY = (float)y;

	m_iWay = Way;
	m_iTeam = iTeam;
	m_fDelay = 0;
	m_iState = MISSILE_MOVE_STATE;
	m_bCrashState = false;
}

void Missile::InitBitMap(BitMap* pBitMap, int iLen)
{
	m_pMissile[iLen] = pBitMap;
}
void Missile::InitExplosion(BitMap* pBitMap, int iLen)
{
	m_pExplosion[iLen] = pBitMap;
}

HDC Missile::Draw()
{
	if (m_iState == MISSILE_MOVE_STATE)
		return m_pMissile[m_iWay]->Draw();
	else
		return m_pExplosion[m_iState - 1]->Draw();
}
POINT Missile::ReturnHbitmap()
{
	BITMAP bmp_info;
	if(m_iState == MISSILE_MOVE_STATE)
		GetObject(m_pMissile[m_iWay]->ReturnHbitmap(), sizeof(bmp_info), &bmp_info);
	else
		GetObject(m_pExplosion[m_iState]->ReturnHbitmap(), sizeof(bmp_info), &bmp_info);

	POINT Pos = { bmp_info.bmWidth, bmp_info.bmHeight };
	return Pos;
}

void Missile::Move(float Distance)
{
	if (m_bCrashState)
		return;

	int Speed;
	if (m_iTeam == PLAYER)
		Speed = 250;
	else
		Speed = 150;

	if (m_iWay == 0)
		m_iX = m_iX - Speed * Distance;
	else if (m_iWay == 1)
		m_iY = m_iY - Speed * Distance;
	else if (m_iWay == 2)
		m_iX = m_iX + Speed * Distance;
	else if (m_iWay == 3)
		m_iY = m_iY + Speed * Distance;
}

void Missile::Explosion()
{
	if (m_bCrashState)
		return;

	m_iX -= 14;
	m_iY -= 10;
	m_iState = MISSILE_EXPLOSION_00_STATE;
	m_bCrashState = true;		
}

bool Missile::ExplosionMotion(float m_fElapseTime)
{
	if (m_bCrashState == false)
		return false;

	m_fDelay = m_fDelay + 100 * m_fElapseTime;
	if (m_fDelay > 10)
	{
		m_fDelay = 0;

		if (m_iState == MISSILE_EXPLOSION_02_STATE)
			return true;
		
		if(m_iState == MISSILE_EXPLOSION_00_STATE)
			m_iState = MISSILE_EXPLOSION_01_STATE;
		else if (m_iState == MISSILE_EXPLOSION_01_STATE)
			m_iState = MISSILE_EXPLOSION_02_STATE;
	}

	return false;
}

void Missile::Remove()
{
	m_iX = 2000;
	m_iY = 2000;
}