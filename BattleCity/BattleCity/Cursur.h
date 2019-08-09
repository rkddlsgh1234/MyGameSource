#pragma once
#include "BitMap.h"

enum CUSUR_MOVE_STATE
{
	CUSUR_STATE_START,
	CUSUR_STATE_MAPTOOL,
};

class BitMap;
class Cursur
{
private:
	int m_iX;
	int m_iY;
	int m_iMainPos;

	int m_iState;

	BitMap* m_pPlayer;
public:
	Cursur();
	~Cursur();

	void Init(int x, int y);
	void InitBitMap(BitMap* pBitMap);

	POINT ReturnHbitmap();
	HDC Draw();

	void TankCursur(int KeyType);

	virtual inline POINT GetPoint()
	{
		POINT Pos = { (LONG)m_iX, (LONG)m_iY };
		return Pos;
	}
	virtual inline int GetState()
	{
		return m_iState;
	}
};

