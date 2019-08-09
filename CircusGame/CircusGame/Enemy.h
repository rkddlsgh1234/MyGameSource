#pragma once
#include <Windows.h>

enum ENEMY_MOVE_STATE
{
	ENEMY_STATE_RUN_00,
	ENEMY_STATE_RUN_01,
};
class BitMap;
class Enemy
{
private:
	float m_iX;
	float m_iY;
	int m_iDelay;
	int m_iNum;

	bool m_bCheckScore;
	bool m_bStop;
	bool m_bSameScore;
	int m_iEnemyState;
	BitMap* m_pBitMap00;
	BitMap* m_pBitMap01;
public:
	Enemy();
	~Enemy();

	void Init(BitMap * pBitMap00, BitMap * pBitMap01, int x, int y, int Num);
	HDC Draw();

	void MoveMotion();
	virtual void Move(float Distance, int Type);
	virtual void MoveScroll(int iState);
	HBITMAP ReturnHbitmap();

	void PlusScore();
	void DeleteScore();
	void Reset(float x);

	void StopEnemy();
	void MoveSet();


	virtual inline int GetNum()
	{
		return m_iNum;
	}
	virtual inline bool GetDelete()
	{
		return m_bStop;
	}
	virtual inline POINT GetPoint()
	{
		POINT Pos = { (LONG)m_iX, (LONG)m_iY };
		return Pos;
	}
	virtual inline POINT GetSize()
	{
		POINT Size = { 0, 0 };
		return Size;
	}
	virtual inline bool GetCashItem()
	{
		return false;
	}
	virtual inline bool GetScore()
	{
		return m_bCheckScore;
	}

};

