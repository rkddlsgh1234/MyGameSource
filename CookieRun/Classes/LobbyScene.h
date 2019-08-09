#pragma once

#include "cocos2d.h"
#include "GameManager.h"

#define __LOBBYSCENE_SCENE_H__

#define JELLY_PRICE 3600
#define HP_PRICE 2400
#define MAX_LEVEL 5

using namespace std;

enum GAME_STATE
{
	GAME_STATE_LOBBY,
	GAME_STATE_READY,
};

class LobbyScene : public cocos2d::Scene
{
private:
	GAME_STATE m_eGameState;
	cocos2d::MenuItemSprite* m_pStateButton;

	UserData* m_stUserData;
private:
	// 공용
	cocos2d::Sprite* m_pBackGround;
	cocos2d::Sprite* m_pUICash;
	cocos2d::Sprite* m_pUIMoney;
	cocos2d::Sprite* m_pUIStamina;
	cocos2d::Label* m_pMoneyLabel;
	cocos2d::Vector <cocos2d::MenuItemSprite*> m_vcSelectStateButton;
	
private:
	cocos2d::SpriteFrameCache* m_pCharacterCache;
	cocos2d::SpriteFrameCache* m_pButtonCache;
	cocos2d::SpriteFrameCache* m_pUICache;
	cocos2d::SpriteFrameCache* m_pUIShopCache;
	cocos2d::SpriteFrameCache* m_pShopItemCache;
private:
	// 로비
	cocos2d::Sprite* m_pCharacterBoard;
	cocos2d::Sprite* m_pMainCharacter;
	
	cocos2d::Menu* m_GameButton;

	cocos2d::Vector <cocos2d::Sprite*> m_vcCharacter;
	cocos2d::Vector <cocos2d::Sprite*> m_vcSelectButton;
private:
	// 상점
	cocos2d::Sprite* m_pUIShop;
	cocos2d::Sprite* m_pUIShopItemInfo;
	cocos2d::Sprite* m_pSelectIcon;
	cocos2d::Sprite* m_pBackButton;
	cocos2d::Sprite* m_pShopItemIcon;
	cocos2d::Sprite* m_pShopItem;
	cocos2d::Sprite* m_pButtonBuy;
	cocos2d::Sprite* m_pShopShowCoin;
	cocos2d::Label* m_pShopItemName;
	cocos2d::Label* m_pShopItemText;
	cocos2d::Label* m_pShopItemPrice;
	map <cocos2d::Sprite*, cocos2d::Sprite*> m_mapUIUpgrade;
	map <cocos2d::Sprite*, cocos2d::Sprite*> m_mapUIItem;
public:
	LobbyScene();
	~LobbyScene();

	static cocos2d::Scene* createScene();
	virtual bool init();

	void InitGame();
	void InitCache();
	void InitRobby();
	void InitShop();
	void InitSprite();

	void onMouseDown(cocos2d::Event* event);
	void SelectButton(cocos2d::Sprite* pSprite);
	void SelectGameState(Ref* pSender);

	void ChangeUI(GAME_STATE eGameState);
	void SetVisibleLobbyObject(bool bVisible);
	void SetVisibleShopObject(bool bVisible);

	void ShowShopInfo(cocos2d::Sprite* pShopSprite);

	CREATE_FUNC(LobbyScene);
};
