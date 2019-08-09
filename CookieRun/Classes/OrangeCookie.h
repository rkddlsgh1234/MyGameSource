#pragma once
#include "PlayerCookie.h"

class OrangeCookie : public PlayerCookie
{
private:
	bool m_bGetBall;
	bool m_bAttack;
	bool m_bExplosion;
	bool m_bSpawnBall;
	int iUpdataDelay;
	cocos2d::Sprite* m_pBall;
	cocos2d::Sprite* m_pDropBall;
	cocos2d::Scene* m_pScene;
	cocos2d::Vector <cocos2d::Sprite*> m_vcEffect;
	cocos2d::Vector <cocos2d::Sprite*> m_vcExplosionEffect;
	cocos2d::SpriteFrameCache* m_pEffectCache;
	ObjectManager* m_pObjectManager;
public:
	OrangeCookie();
	virtual ~OrangeCookie();

	virtual void Init(cocos2d::Scene* pScene, cocos2d::SpriteFrameCache* pJumpSpriteFrame, ObjectManager* pObject);
	virtual void Input();
	virtual void InputPush();
	virtual void UpdataCharacter();
	virtual void StartGame();
	void UpdataDropBall();
	void GenerateBall();
	void ExplosionBall();
	void SetRotate(cocos2d::Node* origin, cocos2d::Vec2 target);
	void DestroyEffect();
};
