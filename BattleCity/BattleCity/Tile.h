#pragma once
#include <Windows.h>

#define TILE_SIZE_X 33
#define TILE_SIZE_Y 25

#define SMALL_TILE_SIZE_X 17
#define SMALL_TILE_SIZE_Y 12

#define DEMAGES_X 8
#define DEMAGES_Y 6

class BitMap;
class Tile
{
private:
	int m_iX;
	int m_iY;
	int m_iType;
	int m_iHitCountUP;
	int m_iHitCountDOWN;
	int m_iHitCountLEFT;
	int m_iHitCountRIGHT;

	int m_iExplosionState;

	POINT BitMapSize;

	float m_iDelay;
	bool m_bExplosion;

	RECT m_rcSize;
	BitMap* m_pBitMap;
	BitMap* m_pExplosion[5];

	bool m_bCheckGetBitMap;
public:
	Tile();
	~Tile();

	void InitPos(int x, int y);
	void InitBitMap(BitMap * pBitMap, int Type);
	void InitExplosion(BitMap* pBitMap, int iLen);
	HDC Draw();
	POINT ReturnSize();

	void ResetBitMap();
	void SelectDraw(int x, int y, BitMap* pTile, int Type);
	void Remove(int x, int y);
	void TileSize();
	void BreakTile(int Way);
	void SetPos(int x, float y);
	bool SelectTile(int x, int y);
	bool CheckHitCount(int KeyType);
	bool ExplosionMotion(float m_fElapseTime);
	void Explosion();

	bool IsTileState();

	virtual inline POINT GetPoint()
	{
		POINT Pos = { (LONG)m_iX, (LONG)m_iY };
		return Pos;
	}
	virtual inline POINT GetBitMapSize()
	{
		return BitMapSize;
	}
	virtual inline bool GetCheckGetBitMap()
	{
		return m_bCheckGetBitMap;
	}
	virtual inline bool GetState()
	{
		return m_bExplosion;
	}
	virtual inline int GetType()
	{
		return m_iType;
	}
	virtual inline int GetExplosionState()
	{
		return m_iExplosionState;
	}
	virtual inline RECT GetSize()
	{
		return m_rcSize;
	}
};

