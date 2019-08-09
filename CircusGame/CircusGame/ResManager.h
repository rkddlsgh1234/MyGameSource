#pragma once
#include <Windows.h>

#define STAR_SIZE_X 14
#define STAR_SIZE_Y 13
#define BACK_X 67
#define BACK_Y 183
#define MITER_X  86
#define MITER_Y  30
#define END_X 76
#define END_Y 49
#define BACK_NORMAL_X 65
#define BACK_NORMAL_Y 64
#define ICON_X 15
#define ICON_Y 13

enum RES_TYPE
{
	RES_TYPE_BACK,
	RES_TYPE_BACK_DECO,
	RES_TYPE_BACK_NORMAL00,
	RES_TYPE_BACK_NORMAL01,
	RES_TYPE_CASH,
	RES_TYPE_DIE,
	RES_TYPE_OBJECT_END,
	RES_TYPE_ENEMY_00,
	RES_TYPE_ENEMY_A00,
	RES_TYPE_ENEMY_A01,
	RES_TYPE_ENEMY_B00,
	RES_TYPE_ENEMY_B01,
	RES_TYPE_ENEMY_C00,
	RES_TYPE_ENEMY_C01,
	RES_TYPE_ENEMY_C02,
	RES_TYPE_ENEMY_02,
	RES_TYPE_FRONT_00,
	RES_TYPE_FRONT_01,
	RES_TYPE_ICON,
	RES_TYPE_PLAYER_00,
	RES_TYPE_PLAYER_01,
	RES_TYPE_PLAYER_02,
	RES_TYPE_STAR_00,
	RES_TYPE_STAR_01,
	RES_TYPE_STAR_02,
	RES_TYPE_WIN_00,
	RES_TYPE_WIN_01,
	RES_TYPE_BLACKBACKGROUND,
	RES_TYPE_SCORDBOARD,
	RES_TYPE_CASH_A00,
	RES_TYPE_CASH_A01,
	RES_TYPE_CASH_B00,
	RES_TYPE_CASH_B01,
	RES_TYPE_MITER_100,
	RES_TYPE_MITER_90,
	RES_TYPE_MITER_80,
	RES_TYPE_MITER_70,
	RES_TYPE_MITER_60,
	RES_TYPE_MITER_50,
	RES_TYPE_MITER_40,
	RES_TYPE_MITER_30,
	RES_TYPE_MITER_20,
	RES_TYPE_MITER_10,
	RES_TYPE_MITER_00,
	RES_TYPE_TITLE_LOGO,
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

