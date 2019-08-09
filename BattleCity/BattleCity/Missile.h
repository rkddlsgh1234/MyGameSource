#pragma once
#include <Windows.h>

using namespace std;

enum MISSILE_MOVE_STATE
{
	MISSILE_MOVE_STATE,
	MISSILE_EXPLOSION_00_STATE,
	MISSILE_EXPLOSION_01_STATE,
	MISSILE_EXPLOSION_02_STATE,
};

enum MISSILE_WAY
{
	WAY_LEFT,
	WAY_UP,
	WAY_RIGHT,
	WAY_DOWN,
};

class BitMap;
class Missile
{
private:
	float m_iX;
	float m_iY;

	float m_fDelay;
	enum MISSILE_WAY m_iWay;
	enum MISSILE_MOVE_STATE m_iState;
	int m_iTeam;

	bool m_bCrashState;
		
	BitMap* m_pExplosion[3];
	BitMap* m_pMissile[4];
public:
	Missile();
	~Missile();

	void Init(float x, float y, MISSILE_WAY Way, int iTeam);
	void InitBitMap(BitMap* pBitMap, int iLen);
	void InitExplosion(BitMap* pBitMap, int iLen);
	HDC Draw();
	POINT ReturnHbitmap();

	void Move(float Distance);
	void Explosion();
	bool ExplosionMotion(float m_fElapseTime);
	void Remove();

	virtual inline POINT GetPoint()
	{
		POINT Pos = { (LONG)m_iX, (LONG)m_iY };
		return Pos;
	}
	virtual inline int GetWay()
	{
		return m_iWay;
	}
	virtual inline bool GetCrashState()
	{
		return m_bCrashState;
	}
};

