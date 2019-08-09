#pragma once
#include <Windows.h>

#define TANK_SIZE_X 33
#define TANK_SIZE_Y 25

#define SHIELD_SIZE_X 33
#define SHIELD_SIZE_Y 25

#define PLAYER 100
#define ENEMY 101

enum TANK_MOVE_STATE
{
	TANK_STATE_DOWN_00,
	TANK_STATE_DOWN_01,
	TANK_STATE_LEFT_00,
	TANK_STATE_LEFT_01,
	TANK_STATE_RIGHT_00,
	TANK_STATE_RIGHT_01,
	TANK_STATE_UP_00,
	TANK_STATE_UP_01,
};

enum TANK_EXPLOSION_STATE
{
	TANK_STATE_EXPLOSION_00,
	TANK_STATE_EXPLOSION_01,
	TANK_STATE_EXPLOSION_02,
	TANK_STATE_EXPLOSION_03,
	TANK_STATE_EXPLOSION_04,
};
enum TANK_SHIELD_STATE
{
	TANK_STATE_SHIELD_00,
	TANK_STATE_SHIELD_01,
};
enum TANK_SPAWN_STATE
{
	TANK_STATE_SPAWN_00,
	TANK_STATE_SPAWN_01,
	TANK_STATE_SPAWN_02,
	TANK_STATE_SPAWN_03,
};

class BitMap;
class Missile;
class Tank
{
private:
	enum TANK_MOVE_STATE m_eMoveState;
	enum TANK_SHIELD_STATE m_eShieldState;
	enum TANK_SPAWN_STATE m_eSpawnState;
	enum TANK_EXPLOSION_STATE m_eExplosionState;
	int m_iTeam;
	int m_iState;
	int m_iKeyState;
	int m_iRandomDelayTwo;

	bool m_bLifeMissile;
	bool m_bLife;
	bool m_bExplosion;
	bool m_bShield;
	bool m_bSpawnMotion;
	bool m_bShowScore;
	float m_fX;
	float m_fY;

	float m_fDelay;
	float m_fShieldTime;
	float m_fSpawnDelay;
	float m_fWayDelay;
	float m_fShieldMotion;

	float m_fRandomDelay;
	float m_fMotionDelay;
	float m_fScoreDelay;

	int m_iCount;

	Missile* m_pMissile;

	BitMap* m_pPlayer[8];
	BitMap* m_pExplosion[5];
	BitMap* m_pMissileBitMap[4];
	BitMap* m_pShield[2];
	BitMap* m_pSpawn[4];
public:
	Tank();
	~Tank();

	void Init(float x, float y, int Team);

	void InitBitMap(BitMap* pBitMap, int iLen);
	void InitExplosion(BitMap* pBitMap, int iLen);
	void InitMissile(BitMap* pBitMap, int iLen);
	void InitShield(BitMap* pBitMap, int iLen);
	void InitSpawn(BitMap* pBitMap, int iLen);
	void Release();
	POINT ReturnSize();
	POINT ReturnSizeShield();
	HDC Draw();
	HDC DrawShield();

	void Move(float ElapseTime, int KeyState);
	void Attack();
	void Reload();
	void ChangeWay(int Way, float ElapseTime);

	void EnemyCrash();
	void Die();
	void ReSpawn();
	virtual int MoveTo(float fElapseTime);
	virtual void AttackTo(float fElapseTime);
	bool ExplosionMotion(float fElapseTime);
	void Explosion();

	void ShieldMotion(float fElapseTime);
	void SpawnMotion(float fElapseTime);
	void MoveMotion(float ElapseTime);
	void ScoreDelay(float ElapseTime);

	void ResetScore();
	void SetPos(int x, int y);
	void MoveBack(float ElapseTime, int KeyState);
	void TurnWay(TANK_MOVE_STATE Way);

	bool CheckState();

	virtual inline POINT GetPoint()
	{
		POINT Pos = { (LONG)m_fX, (LONG)m_fY };
		return Pos;
	}
	virtual inline bool GetLifeMissile()
	{
		return m_bLifeMissile;
	}
	virtual inline bool GetLife()
	{
		return m_bLife;
	}
	virtual inline bool GetExplosion()
	{
		return m_bExplosion;
	}
	virtual inline TANK_MOVE_STATE GetState()
	{
		return m_eMoveState;
	}
	virtual inline int GetTeam()
	{
		return m_iTeam;
	}
	virtual inline Missile* GetMissile()
	{
		return m_pMissile;
	}
	virtual inline bool GetShield()
	{
		return m_bShield;
	}
	virtual inline bool GetShowScore()
	{
		return m_bShowScore;
	}
	virtual inline bool GetSpawnState()
	{
		return m_bSpawnMotion;
	}
};

