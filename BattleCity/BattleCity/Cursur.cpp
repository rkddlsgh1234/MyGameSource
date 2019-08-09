#include "Cursur.h"
#include "ResManager.h"


Cursur::Cursur()
{
}


Cursur::~Cursur()
{
}

void Cursur::Init(int x, int y)
{
	m_iX = x;
	m_iY = y;
	m_iMainPos = y;

	m_iState = VK_UP;
}
void Cursur::InitBitMap(BitMap* pBitMap)
{
	m_pPlayer = pBitMap;
}
HDC Cursur::Draw()
{
	return m_pPlayer->Draw();
}

POINT Cursur::ReturnHbitmap()
{
	BITMAP bmp_info;
	GetObject(m_pPlayer->ReturnHbitmap(), sizeof(bmp_info), &bmp_info);
	POINT Pos = { bmp_info.bmWidth, bmp_info.bmHeight };
	return Pos;
}

void Cursur::TankCursur(int KeyType)
{
	if (m_iState == KeyType)
		return;

	m_iState = KeyType;

	if (m_iState == VK_UP)
		m_iY = m_iMainPos;
	else if (m_iState == VK_DOWN)
		m_iY = m_iMainPos + 40;
}
