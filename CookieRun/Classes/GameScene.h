#pragma once
#include "cocos2d.h"
#include "ObjectManager.h"
#include "PlayerCookie.h"
#include "OrangeCookie.h"
#include "PeachCookie.h"

#define __GAMESCENE_SCENE_H__

using namespace std;


class GameScene : public cocos2d::Scene
{
private:
	PlayerCookie* m_pPlayer;
	ObjectManager* m_pObjectManager;

	bool m_bMousePush;
	bool m_bGameStart;
	bool m_bEndGame;
	bool m_bResult;
	bool m_bFever;
	int m_iResultCoin;
	int m_iResultJelly;
private:
	cocos2d::Sprite* m_pEffectHit;
	cocos2d::Sprite* m_pEffectHealthHit;
	cocos2d::Sprite* m_pJumpButton;
	cocos2d::Sprite* m_pSlideButton;
	cocos2d::Sprite* m_pGetCoin;
	cocos2d::Sprite* m_pHealth;
	cocos2d::Sprite* m_pHealthEmpty;
	cocos2d::Sprite* m_pHealthIcon;
	cocos2d::Sprite* m_pHealthShowMax;
	cocos2d::Sprite* m_pResultCoin;
	cocos2d::Sprite* m_pResultBoard;
	cocos2d::Sprite* m_pResultTitle;
	cocos2d::Sprite* m_pResultJelly;
	cocos2d::Sprite* m_pResultExitButton;
	cocos2d::Sprite* m_pGetJelly;
	cocos2d::Sprite* m_pBonus;
	cocos2d::Label* m_pResultJellyLabel;
	cocos2d::Label* m_pGetJellyLabel;
	cocos2d::Label* m_pResultCoinLabel;
	cocos2d::Label* m_pGetCoinLabel;
	cocos2d::Vector <cocos2d::Sprite*> m_vcBounsTime;
private:
	cocos2d::SpriteFrameCache* m_pCharacterAni;
	cocos2d::SpriteFrameCache* m_pPlayingCache;
	cocos2d::SpriteFrameCache* m_pResultCache;
public:
	GameScene();
	~GameScene();

	static cocos2d::Scene* createScene();
	virtual bool init();
	void onMouseDown(cocos2d::Event* event);
	void onMouseUp(cocos2d::Event* event);

	void InitGame();
	void InitCache();
	void InitPlayer();

	void Updata(float DelayTime);
	void UpdataDelay(float DelayTime);
	void ResultUpdata(float DelayTime);
	void SetItemBig();
	void SetItemSpeed();
	void SetGodMode();

	void SetGod();

	void ReadyUpdata(float DelayTime);

	bool SetHealth();
	void SetHealLife(int Heal);

	void StartGame(float DelayTime);
	void CheckItem();
	void CheckCrash();
	void ChangePlayerGodMode(float DelayTime);
	void ShowScore(float DelayTime);
	void EndScene();

	void UnFeverMode();
	void CheckBonusItem();
	void UpdataFever();

	CREATE_FUNC(GameScene);
};