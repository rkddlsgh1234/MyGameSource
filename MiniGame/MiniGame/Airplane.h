#pragma once
#include <BitMap.h>

#define AIRPLANE_SIZE 73

#define RESPAWN_DELAY 14

enum AIRPLANE_ANIMATION_STATE
{
	ANIMATION_ONE,
	ANIMATION_TWO,
};
enum AIRPLANE_STATE
{
	AIRPLANE_STATE_LIFE,
	AIRPLANE_STATE_EXPLOSION,
	AIRPLANE_STATE_DEAE,
	AIRPLANE_STATE_RESPAWN,
};

class Airplane
{
private:
	Engine::BitMap* m_pBitMap[2];
	Engine::BitMap* m_pExplosion[10];
	float m_fX;
	float m_fY;
	int m_iExplosionAni;
	int m_iRespawnCount;

	float m_fAnimationDelay;
	float m_fExplosionDelay;
	float m_fRespawnDelay;

	bool m_bRespawn;
	AIRPLANE_ANIMATION_STATE m_eAirplane;
	AIRPLANE_STATE m_eAirplaneState;
public:
	Airplane();
	~Airplane();

	void Init(float x, float y);
	void Draw();
	
	void Move(float x, float y);
	void Animation(float fElapseTime);
	void Explosion();

	virtual inline RECT GetRect()
	{
		RECT rc = { (LONG)m_fX + 20, (LONG)m_fY + 20, (LONG)m_fX + AIRPLANE_SIZE - 20, (LONG)m_fY + AIRPLANE_SIZE - 20 };
		return rc;
	}

	virtual inline AIRPLANE_STATE GetState()
	{
		return m_eAirplaneState;
	}
};

