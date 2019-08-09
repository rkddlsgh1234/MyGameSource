#include "Tile.h"
#include "BitMap.h"
#include "resource.h"
#include "ResManager.h"
#include "Tank.h"
#include "Missile.h"

Tile::Tile()
{
}


Tile::~Tile()
{
}

void Tile::InitPos(int x, int y)
{
	m_iX = x;
	m_iY = y;
	m_bCheckGetBitMap = false;
	m_bExplosion = false;
	m_iDelay = 0;
	m_pBitMap = NULL;
	m_iType = 100;
	m_iExplosionState = TANK_STATE_EXPLOSION_02;

	m_iHitCountUP = 0;
	m_iHitCountDOWN = 0;
	m_iHitCountLEFT = 0;
	m_iHitCountRIGHT = 0;

	BitMapSize = { (LONG)TILE_SIZE_X, (LONG)TILE_SIZE_Y };
}

void Tile::InitBitMap(BitMap * pBitMap, int Type)
{
	m_bCheckGetBitMap = true;
	m_iType = Type;
	m_pBitMap = pBitMap;
	TileSize();

	if (Type == BLOCK_14 && m_bExplosion)
	{
		m_iX += 20;
		m_iY += 20;
		m_bExplosion = false;
	}
}

void Tile::InitExplosion(BitMap* pBitMap, int iLen)
{
	m_pExplosion[iLen] = pBitMap;
}
void Tile::ResetBitMap()
{
	if (m_iType == BLOCK_15)
		return;

	m_iType = 100;
	m_pBitMap = NULL;
	m_bCheckGetBitMap = false;
}

HDC Tile::Draw()
{
	if (m_bExplosion == true)
		return m_pExplosion[m_iExplosionState]->Draw();
	else
		return m_pBitMap->Draw();
}


POINT Tile::ReturnSize()
{
	BITMAP bmp_info;
	POINT Pos;
	if (m_bExplosion == true)
	{
		GetObject(m_pExplosion[m_iExplosionState]->ReturnHbitmap(), sizeof(bmp_info), &bmp_info);
		Pos = { bmp_info.bmWidth, bmp_info.bmHeight };
	}
	else
		Pos = { BitMapSize.x, BitMapSize.y };

	
	return Pos;
}


void Tile::SelectDraw(int x, int y, BitMap* pTile, int Type)
{
	if (m_bCheckGetBitMap)
		return;

	POINT pt = { x , y };
	RECT rc = { (LONG)m_iX , (LONG)m_iY , (LONG)m_iX + TILE_SIZE_X , (LONG)m_iY + TILE_SIZE_Y };

	if (PtInRect(&rc, pt))
	{
		m_pBitMap = pTile;
		m_iType = Type;
		m_bCheckGetBitMap = true;
	}
}

bool Tile::SelectTile(int x, int y)
{
	POINT pt = { x , y };
	RECT rc = { (LONG)m_iX , (LONG)m_iY , (LONG)m_iX + TILE_SIZE_X , (LONG)m_iY + TILE_SIZE_Y };

	if (PtInRect(&rc, pt))
		return true;

	return false;
}

void Tile::Remove(int x, int y)
{
	if (m_bCheckGetBitMap == false)
		return;
	if (m_pBitMap->GetType() == IDB_BITMAP46)
		return;

	POINT pt = { x , y };
	RECT rc = { (LONG)m_iX , (LONG)m_iY , (LONG)m_iX + 33 , (LONG)m_iY + 25 };

	if (PtInRect(&rc, pt))
	{
		m_pBitMap = NULL;
		m_iType = 100;
		m_bCheckGetBitMap = false;
	}
}

void Tile::TileSize()
{
	LONG Up = (LONG)m_iX;
	LONG Left = (LONG)m_iY;
	LONG Right = (LONG)m_iX + TILE_SIZE_X;
	LONG Down = (LONG)m_iY + TILE_SIZE_Y;
	if (m_iType == BLOCK_01 || m_iType == BLOCK_09)
	{
		Up = (LONG)m_iX;
		Left = (LONG)m_iY;
		Right = (LONG)m_iX + TILE_SIZE_X;
		Down = (LONG)m_iY + TILE_SIZE_Y / 2 + 1;

		BitMapSize = { (LONG)TILE_SIZE_X, (LONG)TILE_SIZE_Y / 2 + 1 };
	}
	else if (m_iType == BLOCK_02 || m_iType == BLOCK_10)
	{
		Up = (LONG)m_iX + TILE_SIZE_X / 2;
		Left = (LONG)m_iY;
		Right = (LONG)m_iX + TILE_SIZE_X;
		Down = (LONG)m_iY + TILE_SIZE_Y;
		BitMapSize = { (LONG)TILE_SIZE_X - 1, (LONG)TILE_SIZE_Y };
	}
	else if (m_iType == BLOCK_03 || m_iType == BLOCK_11)
	{
		Up = (LONG)m_iX;
		Left = (LONG)m_iY + 13;
		Right = (LONG)m_iX + TILE_SIZE_X;
		Down = (LONG)m_iY + TILE_SIZE_Y;
		BitMapSize = { (LONG)TILE_SIZE_X, (LONG)TILE_SIZE_Y };
	}
	else if (m_iType == BLOCK_04 || m_iType == BLOCK_12)
	{
		Up = (LONG)m_iX;
		Left = (LONG)m_iY;
		Right = (LONG)m_iX + TILE_SIZE_X / 2;
		Down = (LONG)m_iY + TILE_SIZE_Y;
		BitMapSize = { (LONG)TILE_SIZE_X / 2, (LONG)TILE_SIZE_Y };
	}
	
	m_rcSize = { Up, Left, Right, Down};
}

void Tile::BreakTile(int Way)
{
	if (m_iType == BLOCK_15 || m_iType >= BLOCK_08 && m_iType <= BLOCK_11)
		return;
	if (m_bCheckGetBitMap != true)
		return;

	if (Way == WAY_UP)
	{
		BitMapSize.y -= DEMAGES_Y;

		m_iHitCountUP++;

		if (m_iHitCountUP == 2 || m_iHitCountUP == 4)
			m_rcSize.bottom -= 12;
	}
	else if (Way == WAY_DOWN)
	{
		BitMapSize.y -= DEMAGES_Y;
		m_iY += DEMAGES_Y;

		m_iHitCountDOWN++;

		if (m_iHitCountDOWN == 2 || m_iHitCountDOWN == 4)
			m_rcSize.top += DEMAGES_Y * 2;
	}
	else if (Way == WAY_LEFT)	
	{
		BitMapSize.x -= DEMAGES_X;

		m_iHitCountLEFT++;

		if (m_iHitCountLEFT == 2 || m_iHitCountLEFT == 4)
			m_rcSize.right -= DEMAGES_X * 2;
	}
	else if (Way == WAY_RIGHT)
	{
		BitMapSize.x -= DEMAGES_X;
		m_iX += DEMAGES_X;

		m_iHitCountRIGHT++;

		if (m_iHitCountRIGHT == 2 || m_iHitCountRIGHT == 4)
			m_rcSize.left += DEMAGES_X * 2;
	}

	if (m_iType == BLOCK_01 || m_iType == BLOCK_09)
	{
		if (m_iHitCountDOWN + m_iHitCountUP == 2 || m_iHitCountLEFT + m_iHitCountRIGHT == 4)
			m_bCheckGetBitMap = false;
	}
	else if (m_iType == BLOCK_02 || m_iType == BLOCK_10)
	{
		if (m_iHitCountDOWN + m_iHitCountUP == 4 || m_iHitCountLEFT + m_iHitCountRIGHT == 2)
			m_bCheckGetBitMap = false;
	}
	else if (m_iType == BLOCK_03 || m_iType == BLOCK_11)
	{
		if (m_iHitCountDOWN + m_iHitCountUP == 2 || m_iHitCountLEFT + m_iHitCountRIGHT == 4)
			m_bCheckGetBitMap = false;
	}
	else if (m_iType == BLOCK_04 || m_iType == BLOCK_12)
	{
		if (m_iHitCountDOWN + m_iHitCountUP == 4 || m_iHitCountLEFT + m_iHitCountRIGHT == 2)
			m_bCheckGetBitMap = false;
	}
	else if(m_iHitCountLEFT + m_iHitCountRIGHT == 4 || m_iHitCountDOWN + m_iHitCountUP == 4)
		m_bCheckGetBitMap = false;
}	

void Tile::SetPos(int x, float y)
{
	m_iX = (int)x;
	m_iY = (int)y;
}


void Tile::Explosion()
{
	if (m_bExplosion)
		return;

	m_iExplosionState = TANK_STATE_EXPLOSION_00;
	m_bExplosion = true;
}
bool Tile::ExplosionMotion(float m_fElapseTime)
{
	if (m_bExplosion != true)
		return false;

	m_iDelay = m_iDelay + 70 * m_fElapseTime;
	if (m_iDelay > 10)
	{
		m_iDelay = 0;

		m_iExplosionState++;

		if (m_iExplosionState == TANK_STATE_EXPLOSION_04)
			return true;

		if (m_iExplosionState == TANK_STATE_EXPLOSION_02)
		{
			m_iX -= 22;
			m_iY -= 22;
		}
	}

	return false;
}

bool Tile::IsTileState()
{
	if (m_iType == 100)
		return false;
	if (m_bCheckGetBitMap == false)
		return false;

	return true;
}

bool Tile::CheckHitCount(int KeyType)
{
	if (KeyType == VK_UP)
	{
		if (m_iHitCountRIGHT == 1 || m_iHitCountRIGHT == 3)
			return true;
	}
	else if (KeyType == VK_DOWN)
	{
		if (m_iHitCountRIGHT == 1 || m_iHitCountRIGHT == 3)
			return true;
	}
	return false;
}