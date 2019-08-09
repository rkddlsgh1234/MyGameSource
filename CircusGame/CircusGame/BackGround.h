#pragma once
#include <Windows.h>
#include "ResManager.h"

enum BACKGROUND_MOVE_STATE
{
	BACKGROUND_STATE_RUN_00,
	BACKGROUND_STATE_RUN_01,

};
class BitMap;
class BackGround
{
private:
	int TileType;
	int m_iDelay;
	int m_iState;
	float m_iX;
	float m_iY;
	BitMap* m_pBitMap[2];
public:
	BackGround();
	~BackGround();

	virtual void Init(BitMap * pBitMap00, BitMap * pBitMap01, int x, int y, int Type);
	HBITMAP ReturnHbitmap();
	HDC Draw();

	void MoveScroll(int MoveStatei);
	void MoveMotion();
	void SetPos(int x);
	void Reset();

	virtual inline POINT GetPoint()
	{
		POINT Pos = { (LONG)m_iX, (LONG)m_iY };
		return Pos;
	}
	virtual inline int GetType()
	{
		return TileType;
	}
	virtual inline int GetBitMap()
	{
		return RES_TYPE_BACK;
	}
	virtual inline POINT GetSize()
	{
		POINT Size = { 0, 0 };
		return Size;
	}
};

