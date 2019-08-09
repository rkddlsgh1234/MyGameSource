#pragma once
#include "Enemy.h"

class RedMob : public Enemy
{
private:

public:
	RedMob();
	virtual ~RedMob();

	virtual void Init(cocos2d::Scene* pScene, cocos2d::SpriteFrameCache* pIceCache, cocos2d::SpriteFrameCache* pMobCache, int iMonsterNum);
	virtual void CountHit(cocos2d::EventKeyboard::KeyCode keyCode);

	virtual void MoveEnemy();
	virtual	void MoveIce(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Sprite* pPlayer);
	virtual void InitEnemy(cocos2d::Vec2 VecPos, cocos2d::EventKeyboard::KeyCode keyCode);

	/*
	virtual inline cocos2d::Sprite* GetEnemy()
	{
		return m_pEnemy;
	}
	virtual inline cocos2d::Sprite* GetIce()
	{
		return m_pIce;
	}
	virtual inline cocos2d::EventKeyboard::KeyCode GetWay()
	{
		return m_MobWay;
	}
	virtual inline ENEMY_STATE GetState()
	{
		return m_eEnemyState;
	}
	virtual inline bool GetDie()
	{
		return false;
	}
	*/
};