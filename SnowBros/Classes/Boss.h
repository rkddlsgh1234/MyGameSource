#pragma once

#include "cocos2d.h"

enum BOSS_PATTERN 
{
	BOSS_PATTERN_JUMP,
	BOSS_PATTERN_ATTACK,
	BOSS_PATTERN_FALL,
};

class Boss : public cocos2d::Sprite
{
	cocos2d::SpriteFrameCache* m_pBossCache;
	cocos2d::Sprite* m_pBoss;
	cocos2d::Scene* m_pScene;
	BOSS_PATTERN m_ePattern;
	bool m_bSpawn;
	bool m_bPattern;
	bool m_bFall;
	bool m_bDie;
	int m_iEnemy;
	int m_iHp;
public:
	Boss();
	~Boss();


	void Init(cocos2d::SpriteFrameCache* pBossCache , cocos2d::Scene* pScene);
	void SpawnBoss();
	void CheckTile();
	void SetPattern();
	void SetTexture();
	void Jump();
	void Attack();
	void Fall();
	void RemoveEnemyIndex();
	void SetDamage(int iDamage);
	void SetHitEffect();
	void SetDead();

	virtual inline cocos2d::Sprite* GetBoss()
	{
		return m_pBoss;
	}
	virtual inline BOSS_PATTERN GetPattern()
	{
		return m_ePattern;
	}
	virtual inline bool GetFall()
	{
		return m_bFall;
	}
	virtual inline bool GetDie()
	{
		return m_bDie;
	}
	virtual inline int GetEnemyIndex()
	{
		return m_iEnemy;
	}
	virtual inline int GetHp()
	{
		return m_iHp;
	}
	CREATE_FUNC(Boss);
};