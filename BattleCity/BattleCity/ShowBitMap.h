#pragma once
#include <Windows.h>

class BitMap;
class ShowBitMap
{
private:
	int m_iX;
	int m_iY;
	BitMap* m_pBitMap;
public:
	ShowBitMap();
	~ShowBitMap();

	void Init(int x, int y, BitMap* pBitMap);
	HDC Draw();
	POINT ReturnHbitmap();

	void SetPos(int x, float y);

	virtual inline POINT GetPoint()
	{
		POINT Pos = { (LONG)m_iX, (LONG)m_iY };
		return Pos;
	}
};

