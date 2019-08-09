#pragma once
#include <Windows.h>

class BitMap;
class ShowBitMap
{
private:
	float m_iX;
	float m_iY;
	BitMap* m_pBitMap;
public:
	ShowBitMap();
	~ShowBitMap();

	void Init(BitMap * pBitMap, int x, int y);
	HBITMAP ReturnHbitmap();
	HDC Draw();

	virtual inline POINT GetPoint()
	{
		POINT Pos = { (LONG)m_iX, (LONG)m_iY };
		return Pos;
	}
};

