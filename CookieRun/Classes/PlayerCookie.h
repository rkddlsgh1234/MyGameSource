#pragma once

#include "cocos2d.h"

#include "ObjectManager.h"

class PlayerCookie
{
private:
	cocos2d::Sprite* m_pPlayer;
	cocos2d::SpriteFrameCache* m_pCharacterCache;
	cocos2d::SpriteFrameCache* m_pEffectCache;
	cocos2d::Vector <cocos2d::Sprite*> m_vcEffect;
	cocos2d::Vector <cocos2d::Sprite*> m_vcCoinEffect;
	cocos2d::Scene* m_pScene;
	ObjectManager* m_pObjectManager;
	bool m_bJump;
	bool m_bDubbleJump;
	bool m_bSlide;
	bool m_bDie;
	bool m_bGod;
	bool m_bControl;
	bool m_bItemBig;
	bool m_bItemBoost;
	bool m_bFever;
	bool m_bFeverAction;
	bool m_bPush;
public:
	PlayerCookie();
	virtual ~PlayerCookie();

	virtual void Init(cocos2d::Scene* pScene, cocos2d::SpriteFrameCache* pJumpSpriteFrame, ObjectManager* pObject);
	bool Updata(ObjectManager* pObject);
	virtual void UpdataCharacter();
	virtual void Input();
	virtual void InputPush();
	virtual void StartGame();
	virtual void SetRunAni();
	virtual bool CheckCrash();

	void UpdataItem();
	bool SetGravity(ObjectManager* pObject);

	void InputFever();

	void SetRunAnimate();
	void ResetState();

	void SetVisible();
	void SetHealthCookie();
	void ChangeGodMode();
	void ShowCookie();
	void SetItem(string Item);
	void ResetItem(string Item);
	void ShowEffect();

	void SetFeverMode();
	void UnFeverMode();
	void SetJump();
	void SetDubbleJump();
	void UnPush();
	void ActionFever();

	virtual inline cocos2d::Sprite* GetSprite()
	{
		return m_pPlayer;
	}
	virtual inline bool GetGodMode()
	{
		return m_bGod;
	}
	virtual inline bool GetJumpState()
	{
		if (m_bJump)
			return false;
		if (m_bDubbleJump)
			return false;

		return true;
	}
	virtual inline bool GetFever()
	{
		return m_bFever;
	}
	virtual inline bool GetDubbleJump()
	{
		return m_bDubbleJump;
	}
	virtual inline bool GetJump()
	{
		return m_bJump;
	}
	virtual inline bool GetSlide()
	{
		return m_bSlide;
	}
	virtual inline cocos2d::SpriteFrameCache* GetCache()
	{
		return m_pCharacterCache;
	}
	virtual inline bool GetSkil()
	{
		return false;
	}
};