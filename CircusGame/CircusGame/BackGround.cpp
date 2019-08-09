#include "BackGround.h"
#include "BitMap.h"
#include "Player.h"

BackGround::BackGround()
{
}


BackGround::~BackGround()
{
}

void BackGround::Init(BitMap * pBitMap00, BitMap * pBitMap01, int x, int y, int Type)
{
	TileType = Type;
	m_iX = (float)x;
	m_iY = (float)y + (float)70.0;
	m_iState = BACKGROUND_STATE_RUN_00;
	m_iDelay = 0;
	m_pBitMap[0] = pBitMap00;
	m_pBitMap[1] = pBitMap01;
}

HDC BackGround::Draw()
{
	return m_pBitMap[m_iState]->Draw();
}
HBITMAP BackGround::ReturnHbitmap()
{
	return m_pBitMap[m_iState]->ReturnHbitmap();
}

void BackGround::Reset()
{
	m_iState = BACKGROUND_STATE_RUN_00;
}

void BackGround::MoveMotion() // 애니메이션 모션
{
	m_iDelay++;

	if (m_iDelay > 4)
	{
		m_iState++;

		if (m_iState == 2)
			m_iState = BACKGROUND_STATE_RUN_00;

		m_iDelay = 0;
	}
}

void BackGround::MoveScroll(int MoveState)
{
	if (MoveState == NULL || MoveState == PLAYER_STATE_JUMP)
		return;

	if (MoveState == VK_RIGHT || MoveState == PLAYER_STATE_RIGHTJUMP)
		m_iX -= (float)2.0;
	else if (MoveState == VK_LEFT || MoveState == PLAYER_STATE_LEFTJUMP)
		m_iX += (float)2.0;
}

void BackGround::SetPos(int x)
{
	m_iX += x;
}