#pragma once
#include "cocos2d.h"

class Playerble : public cocos2d::Sprite
{
private:
	bool m_bAnimation;
	bool m_bRun;
	bool m_bFall;
	bool m_bJump;
	bool m_bAttack;
	bool m_bPushIce;
	bool m_bIceRun;

	bool m_bLife;
	bool m_bGod;
	
	int m_iSpeed;
private:
	cocos2d::Sprite* m_pPlayer;
	cocos2d::Sprite* m_pBullet;
	
	cocos2d::SpriteFrameCache* m_pChCache;
	cocos2d::EventKeyboard::KeyCode m_PlayerWay;
	cocos2d::Scene* m_pScene;
public:
	Playerble();
	~Playerble();

	void Init(cocos2d::Scene* pScene);
	void Input(cocos2d::EventKeyboard::KeyCode keyCode);
	void ReleaseInput();
	void MovePlayer(cocos2d::EventKeyboard::KeyCode keyCode);

	void Start();

	void SetAnimation(cocos2d::EventKeyboard::KeyCode keyCode);

	void SetFallAnimation();
	void SetFall();
	void SetIceRunAnimation();
	void ResetFall();
	void ResetJump();
	void ResetAnimation();
	void ResetMotion();
	void ResetAttack();
	void ResetIceRun();

	void SetPushIce();
	void ResetPushIce();
	void SetDead();
	void SetGodMode();
	void SetSpeed();
	void SetLife();

	void ChangeTexture();
	void RespawnNextRound();
	void RespawnRound();
	void ResetState();
	void ChangeGodMode();

	virtual inline bool GetLife()
	{
		return m_bLife;
	}
	virtual inline bool GetGod()
	{
		return m_bGod;
	}
	virtual inline cocos2d::Sprite* GetSprite()
	{
		return m_pPlayer;
	}
	virtual inline bool GetJump()
	{
		return m_bJump;
	}
	virtual inline bool GetAttack()
	{
		return m_bAttack;
	}
	virtual inline cocos2d::Sprite* GetBullet()
	{
		return m_pBullet;
	}
	virtual cocos2d::EventKeyboard::KeyCode GetWay()
	{
		return m_PlayerWay;
	}
	virtual inline bool GetIceRun()
	{
		return m_bIceRun;
	}

	CREATE_FUNC(Playerble);
};