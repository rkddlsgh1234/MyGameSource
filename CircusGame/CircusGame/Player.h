#pragma once
#include <Windows.h>
#include <math.h>
#include "BitMap.h"

#define PI 3.141592
#define PLAYER_X 66
#define PLAYER_Y 63


#define PLAYER_POS_X 80
#define PLAYER_POS_Y 187


enum PLAYER_MOVE_STATE
{
	PLAYER_STATE_WAIT,
	PLAYER_STATE_RUN,
	PLAYER_STATE_DIE,
	PLAYER_STATE_WIN_00,
	PLAYER_STATE_WIN_01,
	PLAYER_STATE_JUMP,
	PLAYER_STATE_RIGHTJUMP,
	PLAYER_STATE_LEFTJUMP,
};

class BitMap;
class Player
{
private:
	int m_iPlayerState;
	int m_iMotionDelay;

	float m_iX[3];
	float m_iY[2];
	float m_iDelayOne;
	float m_iDelayTwo;

	bool m_bCheckJump;
	bool m_bCheckDie;
	bool m_bWinCheck;
	BitMap* m_pPlayerType[8];
public:
	Player();
	~Player();

	void Init(BitMap * pBitMap00, BitMap * pBitMap01, BitMap * pBitMapJump, BitMap* pBitMapEnd, BitMap* pBitMapWin00, BitMap* pBitMapWin01, int x, int y);
	HDC Draw();
	HBITMAP ReturnHbitmap();

	int Jump(int KeyType);
	void MoveMotion();
	void Move(int KeyType);
	void TouchCheckObstacle();
	void CheckWin(int x, int y);
	void WinMotion();
	void Reset();

	virtual inline POINT GetPoint()
	{
		POINT Pos = { (LONG)m_iX[0], (LONG)m_iY[0] };
		return Pos;
	}
	virtual inline int GetState()
	{
		return m_iPlayerState;
	}
	virtual inline int GetWin()
	{
		return m_bWinCheck;
	}
	virtual inline bool GetDie()
	{
		return m_bCheckDie;
	}
};

