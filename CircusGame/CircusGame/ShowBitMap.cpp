#include "ShowBitMap.h"
#include "BitMap.h"

ShowBitMap::ShowBitMap()
{
}


ShowBitMap::~ShowBitMap()
{
}

void ShowBitMap::Init(BitMap * pBitMap, int x, int y)
{
	m_iX = (float)x;
	m_iY = (float)y;
	m_pBitMap = pBitMap;
}

HDC ShowBitMap::Draw()
{
	return m_pBitMap->Draw();
}
HBITMAP ShowBitMap::ReturnHbitmap()
{
	return m_pBitMap->ReturnHbitmap();
}