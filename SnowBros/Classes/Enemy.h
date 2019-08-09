#pragma once

#include "cocos2d.h"

enum ENEMY_STATE
{
	ENEMY_STATE_RUN,
	ENEMY_STATE_STACK_ICE,
	ENEMY_STATE_ICE,
	ENEMY_STATE_ICE_RUN,
};

class Enemy : public cocos2d::Sprite
{
private:
	cocos2d::Sprite* m_pEnemy;
	cocos2d::Sprite* m_pIce;
	cocos2d::Sprite* m_pDelay;
	cocos2d::Sprite* m_pItem;
	cocos2d::SpriteFrameCache* m_pIceCache;
	cocos2d::SpriteFrameCache* m_pMobCache;
	cocos2d::EventKeyboard::KeyCode m_MobWay;
	cocos2d::Scene* m_pScene;
	ENEMY_STATE m_eEnemyState;
	int m_iHit;
	int m_iMonsterNum;
	bool m_bHit;
	bool m_bRun;
	bool m_bIceRun;
	bool m_bRotateRun;
	bool m_bDie;
	bool m_bExplosion;
	bool m_bItem;
	cocos2d::Vec2 m_EnemyVec;
public:
	Enemy();
	virtual ~Enemy();

	virtual void Init(cocos2d::Scene* pScene, cocos2d::SpriteFrameCache* pIceCache, cocos2d::SpriteFrameCache* pMobCache, int iMonsterNum);
	virtual void CountHit(cocos2d::EventKeyboard::KeyCode keyCode);
	virtual void MoveEnemy();
	virtual	void MoveIce(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Sprite * pPlayer);
	virtual void InitEnemy(cocos2d::Vec2 VecPos, cocos2d::EventKeyboard::KeyCode keyCode);
	virtual void ReleasePush();

	void SetIceTexture();
	void SetRunAnimation();
	void UnCountHit();
	void DelayUnIce();

	void FastMove();
	void SlowMove();

	void MoveRepeat();

	void SetIceAnimation();
	void ChangeWay();

	void RotateIce();
	void ResetHit();
	void ResetIceRun();
	void EffectExplosion();
	void DestroyEnemy();
	void DamageIce();
	void DropItem();
	void SetIceRush(cocos2d::EventKeyboard::KeyCode keyCode);
	void DestroyIce();

	virtual inline cocos2d::Sprite* GetEnemy()
	{
		return m_pEnemy;
	}
	virtual inline cocos2d::Sprite* GetItem()
	{
		return m_pItem;
	}
	virtual inline cocos2d::EventKeyboard::KeyCode GetWay()
	{
		return m_MobWay;
	}
	virtual inline ENEMY_STATE GetState()
	{
		return m_eEnemyState;
	}
	virtual inline cocos2d::Sprite* GetIce()
	{
		return m_pIce;
	}
	virtual inline bool GetDie()
	{
		return m_bDie;
	}
	virtual inline bool GetisItem()
	{
		return m_bItem;
	}

	virtual inline void HideEnemy()
	{
		m_pEnemy->setVisible(false);
	}
	virtual inline void ShowEnemy()
	{
		m_pEnemy->setVisible(true);
	}
	CREATE_FUNC(Enemy);
};