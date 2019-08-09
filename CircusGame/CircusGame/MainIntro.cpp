#include "MainIntro.h"



MainIntro::MainIntro()
{
}


MainIntro::~MainIntro()
{
}

void MainIntro::Init(BitMap * pBitMap00, BitMap* pBitMap01, BitMap* pBitMap02, int x, int y, int iState)
{
	m_iX = x;
	m_iY = y;
	m_iState = iState;
	m_iMotionDelay = 0;

	m_pBitMap00 = pBitMap00;
	m_pBitMap01 = pBitMap01;
	m_pBitMap02 = pBitMap02;
}

HDC MainIntro::Draw()
{
	if (m_iState == STAR_TYPE_00)
		return m_pBitMap00->Draw();
	else if (m_iState == STAR_TYPE_01)
		return m_pBitMap01->Draw();
	else if (m_iState == STAR_TYPE_02)
		return m_pBitMap02->Draw();

	return m_pBitMap02->Draw();
}
HBITMAP MainIntro::ReturnHbitmap()
{
	if (m_iState == STAR_TYPE_00)
		return m_pBitMap00->ReturnHbitmap();
	else if (m_iState == STAR_TYPE_01)
		return m_pBitMap01->ReturnHbitmap();
	else if (m_iState == STAR_TYPE_02)
		return m_pBitMap02->ReturnHbitmap();

	return m_pBitMap02->ReturnHbitmap();
}

void MainIntro::Motion()
{	
	if (m_iMotionDelay == 10)
	{
		m_iState++;

		if (m_iState == 3)
			m_iState = STAR_TYPE_00;

		m_iMotionDelay = 0;
	}

	m_iMotionDelay++;
}