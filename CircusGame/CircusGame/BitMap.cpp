#include "BitMap.h"
#include "resource.h"

BitMap::BitMap()
{
}


BitMap::~BitMap()
{
}

void BitMap::Init(HDC hdc, HINSTANCE hInst, int id)
{
	m_hMemDC = CreateCompatibleDC(hdc);
	m_hBitMap = LoadBitmap(hInst, MAKEINTRESOURCE(id));
	m_hOldBitMap = (HBITMAP)SelectObject(m_hMemDC, m_hBitMap);
}
HDC BitMap::Draw()
{
	return m_hMemDC;
}

HBITMAP BitMap::ReturnHbitmap()
{
	return m_hBitMap;
}