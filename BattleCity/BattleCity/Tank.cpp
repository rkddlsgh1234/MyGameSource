#include "Tank.h"
#include "BitMap.h"
#include "Missile.h"
#include "GameManager.h"
#include "ResManager.h"

Tank::Tank()
{
	m_bLife = false;
	m_bLifeMissile = false;
	m_pMissile = new Missile;
}

Tank::~Tank()
{
}

void Tank::Init(float x, float y, int Team)
{
	m_eMoveState = TANK_STATE_UP_00;
	m_eSpawnState = TANK_STATE_SPAWN_00;
	m_eShieldState = TANK_STATE_SHIELD_00;
	m_fX = x;
	m_fY = y;

	m_bSpawnMotion = false;
	m_bShield = false;
	m_bLife = true;
	m_bExplosion = false;
	m_bLifeMissile = false;
	m_bShowScore = false;
	m_fMotionDelay = 0;
	m_fDelay = 0;
	m_fShieldTime = 0;
	m_fRandomDelay = 250;
	m_fShieldMotion = 0;
	m_iRandomDelayTwo = 0;
	m_iCount = 0;
	m_iTeam = Team;
	m_fSpawnDelay = 0;
	m_fWayDelay = 0;
}
void Tank::InitBitMap(BitMap* pBitMap, int iLen)
{
	m_pPlayer[iLen] = pBitMap;
}
void Tank::InitExplosion(BitMap* pBitMap, int iLen)
{
	m_pExplosion[iLen] = pBitMap;
	m_pMissile->InitExplosion(pBitMap, iLen);
}
void Tank::InitMissile(BitMap* pBitMap, int iLen)
{
	m_pMissile->InitBitMap(pBitMap, iLen);
}
void Tank::InitShield(BitMap* pBitMap, int iLen)
{
	m_pShield[iLen] = pBitMap;
}
void Tank::InitSpawn(BitMap* pBitMap, int iLen)
{
	m_pSpawn[iLen] = pBitMap;
}

HDC Tank::Draw()
{
	if (m_bSpawnMotion == false)
		return m_pSpawn[m_eSpawnState]->Draw();

	if (m_bExplosion)
		return m_pExplosion[m_eExplosionState]->Draw();
	else
		return m_pPlayer[m_eMoveState]->Draw();
}
HDC Tank::DrawShield()
{
	return m_pShield[m_eShieldState]->Draw();
}
POINT Tank::ReturnSizeShield()
{
	BITMAP bmp_info;
	GetObject(m_pShield[m_eShieldState]->ReturnHbitmap(), sizeof(bmp_info), &bmp_info);
	POINT Pos = { bmp_info.bmWidth, bmp_info.bmHeight };
	return Pos;
}


POINT Tank::ReturnSize()
{
	BITMAP bmp_info;
	if (m_bSpawnMotion == false)
		GetObject(m_pSpawn[m_eShieldState]->ReturnHbitmap(), sizeof(bmp_info), &bmp_info);
	else if (m_bExplosion)
		GetObject(m_pExplosion[m_eExplosionState]->ReturnHbitmap(), sizeof(bmp_info), &bmp_info);
	else
		GetObject(m_pPlayer[m_eMoveState]->ReturnHbitmap(), sizeof(bmp_info), &bmp_info);

	POINT Pos = { bmp_info.bmWidth, bmp_info.bmHeight };
	return Pos;
}

void Tank::Release()
{
	SAFE_DELETE(m_pMissile);
}

void Tank::Move(float ElapseTime, int KeyState)
{
	if (CheckState())
		return;
	if (KeyState == NULL)
		return;

	if (KeyState == VK_LEFT)
		m_fX = m_fX - 70 * ElapseTime;
	else if (KeyState == VK_RIGHT)
		m_fX = m_fX + 70 * ElapseTime;
	else if (KeyState == VK_UP)
		m_fY = m_fY - 70 * ElapseTime;
	else if (KeyState == VK_DOWN)
		m_fY = m_fY + 70 * ElapseTime;

	if (m_iKeyState == KeyState)
		return;

	else
	{
		if (KeyState == VK_LEFT)
			m_eMoveState = TANK_STATE_LEFT_00;
		else if (KeyState == VK_RIGHT)
			m_eMoveState = TANK_STATE_RIGHT_00;
		else if (KeyState == VK_UP)
			m_eMoveState = TANK_STATE_UP_00;
		else if (KeyState == VK_DOWN)
			m_eMoveState = TANK_STATE_DOWN_00;
	}

	if (KeyState == VK_LEFT)
		m_iKeyState = VK_LEFT;
	else if (KeyState == VK_RIGHT)
		m_iKeyState = VK_RIGHT;
	else if (KeyState == VK_UP)
		m_iKeyState = VK_UP;
	else if (KeyState == VK_DOWN)
		m_iKeyState = VK_DOWN;

	MoveMotion(ElapseTime);
}

void Tank::MoveMotion(float ElapseTime)
{
	m_fMotionDelay = m_fMotionDelay + 70 * ElapseTime;

	if (m_fMotionDelay > 20)
	{
		if (m_eMoveState == TANK_STATE_LEFT_00)
			m_eMoveState = TANK_STATE_LEFT_01;
		else if(m_eMoveState == TANK_STATE_LEFT_01)
			m_eMoveState = TANK_STATE_LEFT_00;

		if (m_eMoveState == TANK_STATE_RIGHT_00)
			m_eMoveState = TANK_STATE_RIGHT_01;
		else if (m_eMoveState == TANK_STATE_RIGHT_01)
			m_eMoveState = TANK_STATE_RIGHT_00;

		if (m_eMoveState == TANK_STATE_DOWN_00)
			m_eMoveState = TANK_STATE_DOWN_01;
		else if (m_eMoveState == TANK_STATE_DOWN_01)
			m_eMoveState = TANK_STATE_DOWN_00;

		if (m_eMoveState == TANK_STATE_UP_00)
			m_eMoveState = TANK_STATE_UP_01;
		else if (m_eMoveState == TANK_STATE_UP_01)
			m_eMoveState = TANK_STATE_UP_00;

		m_fMotionDelay = 0;
	}
}

void Tank::Attack()
{
	if (CheckState())
		return;

	if (m_bLifeMissile)
		return;

	m_bLifeMissile = true;

	if (m_eMoveState == TANK_STATE_RIGHT_00 || m_eMoveState == TANK_STATE_RIGHT_01)
		m_pMissile->Init(m_fX + TANK_SIZE_X, m_fY + 10.0f, WAY_RIGHT, m_iTeam);
	else if (m_eMoveState == TANK_STATE_DOWN_00 || m_eMoveState == TANK_STATE_DOWN_01)
		m_pMissile->Init(m_fX + 14.0f, m_fY + TANK_SIZE_Y, WAY_DOWN, m_iTeam);
	else if (m_eMoveState == TANK_STATE_LEFT_00 || m_eMoveState == TANK_STATE_LEFT_01)
		m_pMissile->Init(m_fX, m_fY + 10.0f, WAY_LEFT, m_iTeam);
	else if (m_eMoveState == TANK_STATE_UP_00 || m_eMoveState == TANK_STATE_UP_01)
		m_pMissile->Init(m_fX + 14.0f, m_fY, WAY_UP, m_iTeam);


}
void Tank::Die()
{
	m_bLife = false;
	m_bExplosion = false;
	m_iCount = 0;
	m_fDelay = 0;
	m_bShowScore = true;
}

void Tank::Reload()
{
	m_bLifeMissile = false;
}

int Tank::MoveTo(float ElapseTime)
{

	return 0;
}

void Tank::AttackTo(float ElapseTime)
{

}

void Tank::TurnWay(TANK_MOVE_STATE Way)
{
	m_eMoveState = Way;
}

void Tank::ChangeWay(int Way, float ElapseTime)
{
	if (CheckState())
		return;
	
	m_fWayDelay = m_fWayDelay + 70 * ElapseTime;

	if (m_fWayDelay > m_iRandomDelayTwo)
	{
		int Random = rand() % 4;

		if (m_eMoveState == TANK_STATE_DOWN_00)
		{
			if (Random == 0)
				m_eMoveState = TANK_STATE_RIGHT_00;
			else if (Random == 1)
				m_eMoveState = TANK_STATE_LEFT_00;
			else if (Random == 2)
				m_eMoveState = TANK_STATE_UP_00;
			else if (Random == 3 || Random == 4)
				m_eMoveState = TANK_STATE_DOWN_00;
		}
		else if (m_eMoveState == TANK_STATE_UP_00)
		{
			if (Random == 0)
				m_eMoveState = TANK_STATE_RIGHT_00;
			else if (Random == 1)
				m_eMoveState = TANK_STATE_LEFT_00;
			else if (Random == 2)
				m_eMoveState = TANK_STATE_DOWN_00;
			else if (Random == 3 || Random == 4)
				m_eMoveState = TANK_STATE_UP_00;
		}
		else if (m_eMoveState == TANK_STATE_RIGHT_00)
		{
			if (Random == 0)
				m_eMoveState = TANK_STATE_UP_00;
			else if (Random == 1)
				m_eMoveState = TANK_STATE_LEFT_00;
			else if (Random == 2)
				m_eMoveState = TANK_STATE_DOWN_00;
			else if (Random == 3 || Random == 4)
				m_eMoveState = TANK_STATE_RIGHT_00;
		}
		else if (m_eMoveState == TANK_STATE_LEFT_00)
		{
			if (Random == 0)
				m_eMoveState = TANK_STATE_RIGHT_00;
			else if (Random == 1)
				m_eMoveState = TANK_STATE_UP_00;
			else if (Random == 2)
				m_eMoveState = TANK_STATE_DOWN_00;
			else if (Random == 3 || Random == 4)
				m_eMoveState = TANK_STATE_LEFT_00;
		}
		
		m_fWayDelay = 0;
		m_iRandomDelayTwo = rand() % 5 + 60;
	}
}

void Tank::EnemyCrash()
{
	if (CheckState())
		return;

	if (m_eMoveState == TANK_STATE_DOWN_00)
		m_eMoveState = TANK_STATE_UP_00;
	else if (m_eMoveState == TANK_STATE_UP_00)
		m_eMoveState = TANK_STATE_DOWN_00;
	else if (m_eMoveState == TANK_STATE_RIGHT_00)
		m_eMoveState = TANK_STATE_LEFT_00;
	else if (m_eMoveState == TANK_STATE_LEFT_00)
		m_eMoveState = TANK_STATE_RIGHT_00;
}

void Tank::ReSpawn()
{
	m_fX = 3 * 33;
	m_fY = 13 * 25;
	m_bLife = true;
	m_bShield = true;
	m_bSpawnMotion = false;
	m_bExplosion = false;
	m_eSpawnState = TANK_STATE_SPAWN_00;
	m_eShieldState = TANK_STATE_SHIELD_00;
}

void Tank::Explosion()
{
	if (m_bExplosion)
		return;

	m_eExplosionState = TANK_STATE_EXPLOSION_00;
	m_bExplosion = true;
	m_fDelay = 0;
}

bool Tank::ExplosionMotion(float m_fElapseTime)
{
	if (m_bExplosion != true)
		return false;

	m_fDelay = m_fDelay + 70 * m_fElapseTime;
	if (m_fDelay > 8)
	{
		m_fDelay = 0;

		if (m_eExplosionState == TANK_STATE_EXPLOSION_04)
			return true;

		if (m_eExplosionState == TANK_STATE_EXPLOSION_00)
			m_eExplosionState = TANK_STATE_EXPLOSION_01;
		else if (m_eExplosionState == TANK_STATE_EXPLOSION_01)
			m_eExplosionState = TANK_STATE_EXPLOSION_02;
		else if (m_eExplosionState == TANK_STATE_EXPLOSION_02)
			m_eExplosionState = TANK_STATE_EXPLOSION_03;
		else if (m_eExplosionState == TANK_STATE_EXPLOSION_03)
			m_eExplosionState = TANK_STATE_EXPLOSION_04;

		if (m_eExplosionState == TANK_STATE_EXPLOSION_02)
		{
			m_fX -= 22;
			m_fY -= 22;
		}
	}

	return false;
}

void Tank::ShieldMotion(float fElapseTime)
{
	if (m_bShield == false)
		return;
	if (m_bSpawnMotion == false)
		return;

	m_fShieldTime = m_fShieldTime + 70 * fElapseTime;

	if (m_fShieldTime > 3)
	{
		m_iCount++;
		m_fShieldTime = 0;

		if (m_eShieldState == TANK_STATE_SHIELD_00)
			m_eShieldState = TANK_STATE_SHIELD_01;
		else
			m_eShieldState = TANK_STATE_SHIELD_00;
			
	}
	if (m_iCount > 80)
	{
		m_iCount = 0;
		m_bShield = false;
	}
}

void Tank::SpawnMotion(float fElapseTime)
{
	if (m_bSpawnMotion)
		return;

	m_fSpawnDelay = m_fSpawnDelay + 100 * fElapseTime;

	if (m_fSpawnDelay > 10)
	{
		m_fSpawnDelay = 0;
		m_iCount++;

		if (m_eSpawnState == TANK_STATE_SPAWN_00)
			m_eSpawnState = TANK_STATE_SPAWN_01;
		else if (m_eSpawnState == TANK_STATE_SPAWN_01)
			m_eSpawnState = TANK_STATE_SPAWN_02;
		else if (m_eSpawnState == TANK_STATE_SPAWN_02)
			m_eSpawnState = TANK_STATE_SPAWN_03;
		else if (m_eSpawnState == TANK_STATE_SPAWN_03)
			m_eSpawnState = TANK_STATE_SPAWN_00;

		
	}
	if (m_iCount == 10)
	{
		m_iCount = 0;
		m_bSpawnMotion = true;
		
		if (m_iTeam == PLAYER)
			m_bShield = true;
		else
			m_bShield = false;
	}
}

bool Tank::CheckState()
{
	if (m_bLife == false)
		return true;
	if (m_bExplosion)
		return true;
	if (m_bSpawnMotion == false)
		return true;


	return false;
}

void Tank::SetPos(int x, int y)
{
	if (x != 0)
		m_fX = x;
	if (y != 0)
		m_fY = y;
}
void Tank::MoveBack(float ElapseTime, int KeyState)
{
	if (KeyState == VK_LEFT)
		m_fX = m_fX - 70 * ElapseTime;
	else if (KeyState == VK_RIGHT)
		m_fX = m_fX + 70 * ElapseTime;
	else if (KeyState == VK_UP)
		m_fY = m_fY - 70 * ElapseTime;
	else if (KeyState == VK_DOWN)
		m_fY = m_fY + 70 * ElapseTime;
}

void Tank::ScoreDelay(float ElapseTime)
{
	if (m_bLife)
		return;

	m_fDelay = m_fDelay + 70 * ElapseTime;

	if (m_fDelay > 10)
	{
		m_fDelay = 0;
		m_iCount++;
	}
	if (m_iCount == 4)
		m_bShowScore = false;
}

void Tank::ResetScore()
{
	m_bShowScore = false;
}