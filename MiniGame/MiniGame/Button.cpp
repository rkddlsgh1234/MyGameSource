#include "Button.h"



Button::Button()
{
}


Button::~Button()
{
}

void Button::Init(Engine::BitMap* pBitMap, float x, float y)
{
	m_pButton = pBitMap;
	m_fX = x;
	m_fY = y;
}

void Button::Draw()
{
	m_pButton->Draw(m_fX, m_fY);
}