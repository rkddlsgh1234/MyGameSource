#pragma once
#include "PlayerCookie.h"

class PeachCookie :public PlayerCookie
{
private:
	bool m_bSkil;

	cocos2d::Sprite* m_pDelay;
	ObjectManager* m_pObjectManager;
	cocos2d::Scene* m_pScene;

	cocos2d::Vector <cocos2d::Sprite*> m_vcBackEffect;
public:
	PeachCookie();
	~PeachCookie();

	virtual void Init(cocos2d::Scene* pScene, cocos2d::SpriteFrameCache* pJumpSpriteFrame, ObjectManager* pObject);
	virtual void Input();
	virtual void UpdataCharacter();
	virtual void StartGame();
	virtual void SetRunAni();
	virtual bool CheckCrash();
	void SetSkil();
	void ResetSkil();

	virtual inline bool GetSkil()
	{
		return m_bSkil;
	}
};