#include "Airplane.h"

Airplane::Airplane()
{
}


Airplane::~Airplane()
{
}

void Airplane::Init(float x, float y)
{
	m_pBitMap[0] = Engine::ResManager::GetInstance()->GetBitMap("Airplane_00.bmp");
	m_pBitMap[1] = Engine::ResManager::GetInstance()->GetBitMap("Airplane_01.bmp");

	m_pExplosion[0] = Engine::ResManager::GetInstance()->GetBitMap("Explosion_00.bmp");
	m_pExplosion[1] = Engine::ResManager::GetInstance()->GetBitMap("Explosion_01.bmp");
	m_pExplosion[2] = Engine::ResManager::GetInstance()->GetBitMap("Explosion_02.bmp");
	m_pExplosion[3] = Engine::ResManager::GetInstance()->GetBitMap("Explosion_03.bmp");
	m_pExplosion[4] = Engine::ResManager::GetInstance()->GetBitMap("Explosion_04.bmp");
	m_pExplosion[5] = Engine::ResManager::GetInstance()->GetBitMap("Explosion_05.bmp");
	m_pExplosion[6] = Engine::ResManager::GetInstance()->GetBitMap("Explosion_06.bmp");
	m_pExplosion[7] = Engine::ResManager::GetInstance()->GetBitMap("Explosion_07.bmp");
	m_pExplosion[8] = Engine::ResManager::GetInstance()->GetBitMap("Explosion_08.bmp");
	m_pExplosion[9] = Engine::ResManager::GetInstance()->GetBitMap("Explosion_09.bmp");

	m_eAirplane = ANIMATION_ONE;
	m_eAirplaneState = AIRPLANE_STATE_LIFE;
	m_fX = x;
	m_fY = y;
	m_iExplosionAni = 0;
	m_iRespawnCount = 0;
	m_fAnimationDelay = 0;
	m_fExplosionDelay = 0;
	m_fRespawnDelay = 0;
	m_bRespawn = false;
}
void Airplane::Draw()
{
	if (m_eAirplaneState == AIRPLANE_STATE_DEAE)
		return;

	if (m_eAirplaneState == AIRPLANE_STATE_LIFE || m_bRespawn)
		m_pBitMap[m_eAirplane]->Draw(m_fX, m_fY);
	else if (m_eAirplaneState == AIRPLANE_STATE_EXPLOSION)
		m_pExplosion[m_iExplosionAni]->Draw(m_fX, m_fY);
}

void Airplane::Move(float x, float y)
{
	if (m_eAirplaneState != AIRPLANE_STATE_LIFE && m_eAirplaneState != AIRPLANE_STATE_RESPAWN)
		return;
	if (y < 110 || y > 700)
	{
		if (x < 0 || x > 470)
			return;
		else
			m_fX = x;
	}
	else if (x < 0 || x > 470)
	{
		if (y < 110 || y > 700)
			return;
		else
			m_fY = y;
	}
	else
	{
		m_fX = x;
		m_fY = y;
	}
}

void Airplane::Animation(float fElapseTime)
{
	if (m_eAirplaneState == AIRPLANE_STATE_DEAE)
		return;

	if (m_eAirplaneState == AIRPLANE_STATE_LIFE)
	{
		m_fAnimationDelay = m_fAnimationDelay + 70 * fElapseTime;

		if (m_fAnimationDelay > 4)
		{
			m_fAnimationDelay = 0;
			if (m_eAirplane == ANIMATION_ONE)
				m_eAirplane = ANIMATION_TWO;
			else if (m_eAirplane == ANIMATION_TWO)
				m_eAirplane = ANIMATION_ONE;
		}
	}
	else if(m_eAirplaneState == AIRPLANE_STATE_EXPLOSION)
	{
		m_fExplosionDelay = m_fExplosionDelay + 70 * fElapseTime;

		if (m_fExplosionDelay > 5)
		{
			m_fExplosionDelay = 0;
			m_iExplosionAni++;
			
			if (m_iExplosionAni == 10)
			{
				m_eAirplaneState = AIRPLANE_STATE_RESPAWN;
				m_iExplosionAni = 0;
				m_fX = 230;
				m_fY = 500;
			}
		}
	}
	else if (m_eAirplaneState == AIRPLANE_STATE_RESPAWN)
	{
		m_fRespawnDelay = m_fRespawnDelay + 70 * fElapseTime;

		if (m_fRespawnDelay > 5)
		{
			m_fRespawnDelay = 0;
			m_iRespawnCount++;
			if (m_bRespawn)
				m_bRespawn = false;
			else
				m_bRespawn = true;
		}

		if (m_iRespawnCount == 10)
		{
			m_iRespawnCount = 0;
			m_eAirplaneState = AIRPLANE_STATE_LIFE;
		}
	}
}

void Airplane::Explosion()
{
	if (m_eAirplaneState != AIRPLANE_STATE_LIFE)
		return;
	m_eAirplaneState = AIRPLANE_STATE_EXPLOSION;
}
