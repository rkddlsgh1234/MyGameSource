#include "ShowBitMap.h"
#include "BitMap.h"


ShowBitMap::ShowBitMap()
{
}


ShowBitMap::~ShowBitMap()
{
}

void ShowBitMap::Init(int x, int y, BitMap* pBitMap)
{
	m_iX = (int)x;
	m_iY = (int)y;
	m_pBitMap = pBitMap;
}

HDC ShowBitMap::Draw()
{
	return m_pBitMap->Draw();
}

POINT ShowBitMap::ReturnHbitmap()
{
	BITMAP bmp_info;
	GetObject(m_pBitMap->ReturnHbitmap(), sizeof(bmp_info), &bmp_info);
	POINT Pos = { bmp_info.bmWidth, bmp_info.bmHeight };
	return Pos;
}

void ShowBitMap::SetPos(int x, float y)
{
	m_iX = (int)x;
	m_iY = (int)y;
}

