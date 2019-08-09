#pragma once
#include <Windows.h>
#pragma comment (lib, "Msimg32.lib")

class BitMap
{
private:
	HDC		m_hMemDC;
	HBITMAP	m_hBitMap;
	HBITMAP	m_hOldBitMap;
	int m_iType;

public:
	void Init(HDC hdc, HINSTANCE hInst, int id);
	HDC Draw();
	HBITMAP ReturnHbitmap();

	virtual inline int GetType()
	{
		return m_iType;
	}

	BitMap();
	~BitMap();
};