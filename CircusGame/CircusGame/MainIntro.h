#pragma once
#include <Windows.h>
#include "BitMap.h"

enum MAIN_MENU_STATE
{
	STAR_TYPE_00,
	STAR_TYPE_01,
	STAR_TYPE_02,
};
class BitMap;
class MainIntro
{
private:
	int m_iX;
	int m_iY;
	int m_iState;
	int m_iMotionDelay;
	BitMap * m_pBitMap00;
	BitMap * m_pBitMap01;
	BitMap * m_pBitMap02;
public:
	MainIntro();
	~MainIntro();

	void Init(BitMap * pBitMap00, BitMap* pBitMap01, BitMap* pBitMap02, int x, int y, int State);
	HDC Draw();
	HBITMAP ReturnHbitmap();

	void Motion();

	virtual inline POINT GetPoint()
	{
		POINT Pos = { (LONG)m_iX, (LONG)m_iY };
		return Pos;
	}
};

