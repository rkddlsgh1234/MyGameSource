#include "Player.h"
#include "GameManager.h"

Player::Player()
{
}


Player::~Player()
{
}

void Player::Init(BitMap * pBitMap00, BitMap * pBitMap01, BitMap * pBitMapJump, BitMap* pBitMapEnd, BitMap* pBitMapWin00, BitMap* pBitMapWin01, int x, int y)
{
	m_bCheckJump = false;
	m_bCheckDie = false;
	m_bWinCheck = false;
	m_iDelayOne = 0;
	m_iMotionDelay = 0;
	m_iPlayerState = PLAYER_STATE_WAIT;
	m_iX[0] = (float)x;
	m_iY[0] = (float)y;
	m_pPlayerType[0] = pBitMap00;
	m_pPlayerType[1] = pBitMap01;
	m_pPlayerType[2] = pBitMapEnd;
	m_pPlayerType[3] = pBitMapWin00;
	m_pPlayerType[4] = pBitMapWin01;
	m_pPlayerType[5] = pBitMapJump;
	m_pPlayerType[6] = pBitMapJump;
	m_pPlayerType[7] = pBitMapJump;

	// 0. 대기		1.달리기		2.장애물밟음		3.승리모션(1)		4.승리모션(2)		5.점프
}

HDC Player::Draw()
{
	return m_pPlayerType[m_iPlayerState]->Draw();
}
HBITMAP Player::ReturnHbitmap()
{
	return m_pPlayerType[m_iPlayerState]->ReturnHbitmap();
}

float DegreeToRadian(float Degree)
{
	return Degree * PI / 180.0f;
}

int Player::Jump(int KeyType)
{
	if (m_bCheckJump == false)
	{
		if (KeyType == NULL)
		{
			m_iPlayerState = PLAYER_STATE_WAIT;
			return NULL;
		}

		if (KeyType == VK_RIGHT && GetKeyState(VK_SPACE) & 0x8000)
		{
			m_iDelayOne = 180;
			m_iDelayTwo = 360;
			m_iPlayerState = PLAYER_STATE_RIGHTJUMP;
			m_bCheckJump = true;
		}
		else if (KeyType == VK_LEFT && GetKeyState(VK_SPACE) & 0x8000)
		{
			m_iDelayOne = 360;
			m_iDelayTwo = 180;
			m_iPlayerState = PLAYER_STATE_LEFTJUMP;
			m_bCheckJump = true;
		}
		else if (KeyType == VK_SPACE)
		{
			m_iDelayOne = 180;
			m_iDelayTwo = 360;
			m_iPlayerState = PLAYER_STATE_JUMP;
			m_bCheckJump = true;
		}

		m_iX[1] = m_iX[0] + PLAYER_X;
		m_iY[1] = m_iY[0];
		if (m_bCheckJump && GameManager::GetInstance()->GetEndLine())
		{
			if (m_iPlayerState == PLAYER_STATE_RIGHTJUMP)
			{
				m_iX[1] = m_iX[0] + PLAYER_X;
				m_iDelayOne = 180;
				m_iDelayTwo = 360;
			}
			else if (m_iPlayerState == PLAYER_STATE_LEFTJUMP)
			{
				m_iX[1] = m_iX[0] - PLAYER_X;
				m_iDelayOne = 360;
				m_iDelayTwo = 180;
			}
		}
		
	}

	if (m_bCheckJump)
	{
		m_iX[2] = (float)(m_iX[1] + 50.0 * cosf(DegreeToRadian(m_iDelayOne)));
		m_iY[0] = (float)(m_iY[1] + 85.0 * sinf(DegreeToRadian(m_iDelayOne)));
		if (m_iPlayerState == PLAYER_STATE_RIGHTJUMP || m_iPlayerState == PLAYER_STATE_JUMP)
			m_iDelayOne += 2.5;
		else if (m_iPlayerState == PLAYER_STATE_LEFTJUMP)
			m_iDelayOne -= 2.5;

		if (m_iDelayOne == m_iDelayTwo)
		{
			m_iPlayerState = PLAYER_STATE_WAIT;
			if (GameManager::GetInstance()->GetEndLine())
			{
				if (m_iPlayerState == PLAYER_STATE_RIGHTJUMP)
					m_iX[0] = m_iX[2] + 25;
				else if (m_iPlayerState == PLAYER_STATE_LEFTJUMP)
					m_iX[0] = m_iX[2] - 25;
			}
				
			m_iY[0] = 187;
			m_iDelayOne = 0;
			m_bCheckJump = false;
		}
		else
			return m_iPlayerState;
	}
	MoveMotion();
	return KeyType;
}

void Player::MoveMotion()
{
	if (m_iMotionDelay > 15)
	{
		m_iPlayerState++;

		if (m_iPlayerState == PLAYER_STATE_RUN + 1)
			m_iPlayerState = PLAYER_STATE_WAIT;

		m_iMotionDelay = 0;
	}
	m_iMotionDelay++;
}
void Player::WinMotion()
{
	if (m_iMotionDelay > 25)
	{
		m_iPlayerState++;

		if (m_iPlayerState == PLAYER_STATE_WIN_01 + 1)
			m_iPlayerState = PLAYER_STATE_WIN_00;

		m_iMotionDelay = 0;
	}
	m_iMotionDelay++;
}

void Player::Reset()
{
	m_iX[0] = 80;
	m_iY[0] = 187;

	m_bCheckJump = false;
	m_bCheckDie = false;
	m_bWinCheck = false;
	m_iDelayOne = 0;
	m_iMotionDelay = 0;
	m_iPlayerState = PLAYER_STATE_WAIT;
}

void Player::Move(int KeyType)
{
	if (KeyType == VK_LEFT || KeyType == PLAYER_STATE_LEFTJUMP)
		m_iX[0] -= (float)2.0;
	else if (KeyType == VK_RIGHT || KeyType == PLAYER_STATE_RIGHTJUMP)
		m_iX[0] += (float)2.0;
}

void Player::TouchCheckObstacle()
{
	if (m_bCheckDie)
	{
		m_bCheckDie = false;
		m_iPlayerState = PLAYER_STATE_WAIT;
		m_iY[0] = 187;
		m_iX[0] = 80.0;
		m_iDelayOne = 0;
		m_iDelayTwo = 0;
	}
	else
	{
		m_bCheckDie = true;
		m_iPlayerState = PLAYER_STATE_DIE;
	}
}

void Player::CheckWin(int x, int y)
{
	m_iMotionDelay = 0;
	m_iPlayerState = PLAYER_STATE_WIN_00;
	m_bWinCheck = true;
	m_iX[0] = x;
	m_iY[0] = 140;
}