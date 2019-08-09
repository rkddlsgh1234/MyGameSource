#pragma once
#include <Windows.h>


enum RES_TYPE
{
	BLOCK_00,
	BLOCK_01,
	BLOCK_02,
	BLOCK_03,
	BLOCK_04,
	BLOCK_05,
	BLOCK_06,
	BLOCK_07,
	BLOCK_08,
	BLOCK_09,
	BLOCK_10,
	BLOCK_11,
	BLOCK_12,
	BLOCK_13,
	BLOCK_14,
	E_DOWN_00,
	E_DOWN_01,
	E_LEFT_00,
	E_LEFT_01,
	E_RIGHT_00,
	E_RIGHT_01,
	E_UP_00,
	E_UP_01,
	ENEMY_ICON,
	EXPLOSION_00,
	EXPLOSION_01,
	EXPLOSION_02,
	EXPLOSION_03,
	EXPLOSION_04,
	MISSILE_00,
	MISSILE_01,
	MISSILE_02,
	MISSILE_03,
	PLAYER_ICON,
	SHIELD_00,
	SHIELD_01,
	STAGE_ICON,
	TANK_DOWN_00,
	TANK_DOWN_01,
	TANK_LEFT_00,
	TANK_LEFT_01,
	TANK_RIGHT_00,
	TANK_RIGHT_01,
	TANK_UP_00,
	TANK_UP_01,
	BLOCK_15,
	TITLE_LOGO,
	SPAWN_00,
	SPAWN_01,
	SPAWN_02,
	SPAWN_03,
	RES_TYPE_END,
};

class BitMap;
class ResManager
{
private:
	BitMap * m_pBitMap[RES_TYPE_END];

public:
	ResManager();
	~ResManager();

	void Init(HDC hdc, HINSTANCE hInst);
	BitMap*	GetBitMap(RES_TYPE resType);
	void Release();
};
