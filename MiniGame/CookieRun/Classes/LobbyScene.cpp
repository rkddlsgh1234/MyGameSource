#include "LobbyScene.h"
#include "GameScene.h"

#include "ResManager.h"
#include "SimpleAudioEngine.h"

#pragma execution_character_set("utf-8")

USING_NS_CC;

#define CHARACTER_SCALE 2
#define MAIN_CHARACTER_POS_X 600
#define MAIN_CHARACTER_POS_Y 230

LobbyScene::LobbyScene()
{
}

LobbyScene::~LobbyScene()
{
}

Scene* LobbyScene::createScene()
{
	auto scene = Scene::create();
	auto layer = LobbyScene::create();
	scene->addChild(layer);

	return scene;
}

bool LobbyScene::init()
{
	auto Mouse_Listener = EventListenerMouse::create();
	Mouse_Listener->onMouseDown = CC_CALLBACK_1(LobbyScene::onMouseDown, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(Mouse_Listener, this);

	InitGame();
	return true;
}

void LobbyScene::InitGame()
{
	m_eGameState = GAME_STATE_LOBBY;
	m_stUserData = GameManager::GetInstance()->GetUserData();

	InitCache();
	InitSprite();
	InitRobby();
	InitShop();
}

void LobbyScene::InitCache()
{
	m_pCharacterCache = SpriteFrameCache::sharedSpriteFrameCache();
	m_pCharacterCache->addSpriteFramesWithFile("main_character.plist");
	m_pButtonCache = SpriteFrameCache::sharedSpriteFrameCache();
	m_pButtonCache->addSpriteFramesWithFile("main_button.plist");
	m_pUICache = SpriteFrameCache::sharedSpriteFrameCache();
	m_pUICache->addSpriteFramesWithFile("ui_stat.plist");
	m_pUIShopCache = SpriteFrameCache::sharedSpriteFrameCache();
	m_pUIShopCache->addSpriteFramesWithFile("ui_shop.plist");
	m_pShopItemCache = SpriteFrameCache::sharedSpriteFrameCache();
	m_pShopItemCache->addSpriteFramesWithFile("shop.plist");
}
void LobbyScene::InitRobby()
{
	m_pCharacterBoard = Sprite::create("main_character_select_board.png");
	m_pCharacterBoard->setPosition(Vec2(ResManager::GetInstance()->GetDisplaySize().width / 3 - 50, ResManager::GetInstance()->GetDisplaySize().height / 2));
	m_pCharacterBoard->setScale(2.0f, 1.3f);
	this->addChild(m_pCharacterBoard, 0);
	//
	char str[100] = { 0 };
	for (int i = 0; i < 4; i++)
	{
		sprintf(str, "main_character_%02d.png", i);
		Sprite* pCharacter = Sprite::createWithSpriteFrameName(str);
		pCharacter->setName(str);
		pCharacter->setTag(i);
		pCharacter->setScale(0.9f);
		pCharacter->setPosition(Vec2(100, 395 - (i * 65)));
		m_vcCharacter.pushBack(pCharacter);
		Sprite* pSelectButton = Sprite::create("select_cookie_button.png");
		pSelectButton->setPosition(pCharacter->getPosition().x + 150, pCharacter->getPosition().y + 8 - (i + 10));
		pSelectButton->setName(pCharacter->getName());
		pSelectButton->setTag(i);
		m_vcSelectButton.pushBack(pSelectButton);
		this->addChild(pCharacter, 2);
		this->addChild(pSelectButton, 3);
	}
	//
	//
	m_pMainCharacter = Sprite::createWithSpriteFrameName("main_character_00.png");
	m_pMainCharacter->setScale(CHARACTER_SCALE);
	m_pMainCharacter->setPosition(Vec2(MAIN_CHARACTER_POS_X, MAIN_CHARACTER_POS_Y));
	this->addChild(m_pMainCharacter, 2);

	auto pCashLabel = Label::createWithTTF("0", "fonts/Anton.ttf", 20);
	pCashLabel->setPosition(Vec2(m_pUICash->getPosition().x, m_pUICash->getPosition().y));
	this->addChild(pCashLabel, 2);

	m_pMoneyLabel = Label::createWithTTF("0", "fonts/Anton.ttf", 20);
	sprintf(str, "%d", GameManager::GetInstance()->GetUserData()->iMoney);
	m_pMoneyLabel->setString(str);
	m_pMoneyLabel->setPosition(Vec2(m_pUIMoney->getPosition().x, m_pUIMoney->getPosition().y));
	this->addChild(m_pMoneyLabel, 2);
}
void LobbyScene::InitShop()
{
	m_pUIShop = Sprite::createWithSpriteFrameName("ui_shop.png");
	m_pUIShop->setPosition(Vec2(ResManager::GetInstance()->GetDisplaySize().width / 3 - 50, ResManager::GetInstance()->GetDisplaySize().height / 2));
	m_pUIShop->setVisible(false);
	this->addChild(m_pUIShop, 0);

	m_pBackButton = Sprite::create("ui_back_button.png");
	m_pBackButton->setPosition(m_pUIShop->getBoundingBox().getMaxX() - 10, m_pUIShop->getBoundingBox().getMaxY() - 5);
	m_pBackButton->setVisible(false);
	this->addChild(m_pBackButton, 1);

	Sprite* pHpUpgrade;
	for (int i = 0; i < 3; i++)
	{
		Sprite* pUIUpgradeIcon = Sprite::createWithSpriteFrameName("ui_upgrade_stat.png");
		pUIUpgradeIcon->setPosition(Vec2(m_pUIShop->getBoundingBox().getMinX() + 75 + (i * pUIUpgradeIcon->getContentSize().width), m_pUIShop->getBoundingBox().getMaxY() - 100));
		pUIUpgradeIcon->setVisible(false);
		Sprite* pUIUpgrade;

		char str[100] = { 0 };
		if (i == 0)
		{
			sprintf(str, "shop_hp_%02d.png", GameManager::GetInstance()->GetUserData()->iHpLevel - 1);
			pUIUpgrade = Sprite::createWithSpriteFrameName(str);
			pUIUpgrade->setName("Health");
		}
		else if (i == 1)
		{
			char str[100] = { 0 };
			sprintf(str, "shop_jelly_%02d.png", GameManager::GetInstance()->GetUserData()->iJellyLevel - 1);
			pUIUpgrade = Sprite::createWithSpriteFrameName(str);
			pUIUpgrade->setName("Jelly");
		}
		else if (i == 2)
		{
			pUIUpgrade = Sprite::createWithSpriteFrameName("shop_time_00.png");
			pUIUpgrade->setName("Time");
		}
		pUIUpgrade->setPosition(Vec2(pUIUpgradeIcon->getPosition().x, pUIUpgradeIcon->getPosition().y));
		pUIUpgrade->setVisible(false);
		pUIUpgrade->setScale(2.0f);
	
		m_mapUIUpgrade.insert(pair<Sprite*, Sprite*>(pUIUpgrade, pUIUpgradeIcon));

		this->addChild(pUIUpgradeIcon, 1);
		this->addChild(pUIUpgrade, 2);

		if (i == 0)
		{
			m_pSelectIcon = Sprite::createWithSpriteFrameName("ui_select_icon.png");
			m_pSelectIcon->setPosition(Vec2(pUIUpgradeIcon->getPosition().x, pUIUpgradeIcon->getPosition().y));
			m_pSelectIcon->setVisible(false);
			this->addChild(m_pSelectIcon, 3);
			pHpUpgrade = pUIUpgrade;
		}
	}
	
	for (int i = 0; i < 6; i++)
	{
		Sprite* pUIItem = Sprite::createWithSpriteFrameName("ui_item.png");
		if (i < 3)
			pUIItem->setPosition(Vec2(m_pUIShop->getBoundingBox().getMinX() + 75 + (i * pUIItem->getContentSize().width), m_pUIShop->getBoundingBox().getMidY() - 37));
		else
			pUIItem->setPosition(Vec2(m_pUIShop->getBoundingBox().getMinX() + 75 + ((i - 3) * pUIItem->getContentSize().width), m_pUIShop->getBoundingBox().getMidY() - 42 - pUIItem->getContentSize().height));
		pUIItem->setVisible(false);

		Sprite* pItem;
		if (i == 0)
		{
			pItem = Sprite::createWithSpriteFrameName("shop_item_hp.png");
			pItem->setName("Hp");
		}
		else if (i == 1)
		{
			pItem = Sprite::createWithSpriteFrameName("shop_item_time.png");
			pItem->setName("Time");
		}
		else if (i == 2)
		{
			pItem = Sprite::createWithSpriteFrameName("shop_item_ext.png");
			pItem->setName("Ext");
		}
		else if (i == 3)
		{
			pItem = Sprite::createWithSpriteFrameName("shop_item_boost.png");
			pItem->setName("Boost");
		}
		else if (i == 4)
		{
			pItem = Sprite::createWithSpriteFrameName("shop_item_relay.png");
			pItem->setName("Relay");
		}
		else if (i == 5)
		{
			pItem = Sprite::createWithSpriteFrameName("shop_item_box.png");
			pItem->setName("Box");
		}

		pItem->setPosition(Vec2(pUIItem->getPosition().x, pUIItem->getPosition().y));
		pItem->setVisible(false);
		pItem->setScale(2.0f);
		m_mapUIItem.insert(pair<Sprite*, Sprite*>(pItem, pUIItem));
		this->addChild(pUIItem, 1);
		this->addChild(pItem, 2);
	}
	m_pUIShopItemInfo = Sprite::createWithSpriteFrameName("ui_item_info.png");
	m_pUIShopItemInfo->setPosition(Vec2(ResManager::GetInstance()->GetDisplaySize().width / 2 + 200, ResManager::GetInstance()->GetDisplaySize().height / 2 + 50));
	m_pUIShopItemInfo->setVisible(false);
	this->addChild(m_pUIShopItemInfo, 3);
	m_pShopItemIcon = Sprite::createWithSpriteFrameName("ui_select_icon.png");
	m_pShopItemIcon->setPosition(Vec2(m_pUIShopItemInfo->getPosition().x - 110, m_pUIShopItemInfo->getPosition().y + 70));
	m_pShopItemIcon->setVisible(false);
	this->addChild(m_pShopItemIcon, 4);
	m_pShopItem = Sprite::createWithSpriteFrame(pHpUpgrade->getDisplayFrame());
	m_pShopItem->setName("Health");
	m_pShopItem->setScale(2);
	m_pShopItem->setPosition(Vec2(m_pShopItemIcon->getPosition().x, m_pShopItemIcon->getPosition().y));
	m_pShopItem->setVisible(false);
	this->addChild(m_pShopItem, 3);

	m_pButtonBuy = Sprite::createWithSpriteFrameName("butto_buy.png");
	m_pButtonBuy->setPosition(Vec2(m_pShopItemIcon->getPosition().x + 150, m_pShopItemIcon->getPosition().y - 50));
	m_pButtonBuy->setVisible(false);
	this->addChild(m_pButtonBuy, 3);

	char str[100] = { 0 };
	sprintf(str, "체력 강화 Lv.%d", GameManager::GetInstance()->GetUserData()->iHpLevel);
	m_pShopItemName = Label::createWithTTF(str, "fonts/Maplestory Bold.ttf", 25);
	m_pShopItemName->setPosition(Vec2(m_pUIShopItemInfo->getPosition().x + 40, m_pUIShopItemInfo->getPosition().y + 95));
	m_pShopItemName->setVisible(false);
	this->addChild(m_pShopItemName, 5);

	sprintf(str, "추가 체력이 %d 늘어납니다", GameManager::GetInstance()->GetUserData()->iHpLevel * 10);
	m_pShopItemText = Label::createWithTTF(str, "fonts/Maplestory Bold.ttf", 15);
	m_pShopItemText->setPosition(Vec2(m_pShopItemIcon->getPosition().x + 80, m_pShopItemIcon->getPosition().y - 80));
	m_pShopItemText->setVisible(false);
	this->addChild(m_pShopItemText, 5);

	sprintf(str, "%d", GameManager::GetInstance()->GetUserData()->iHpLevel * 2400);
	m_pShopItemPrice = Label::createWithTTF(str, "fonts/Maplestory Bold.ttf", 25);
	m_pShopItemPrice->setPosition(Vec2(m_pUIShopItemInfo->getPosition().x + 50, m_pUIShopItemInfo->getPosition().y + 55));
	m_pShopItemPrice->setVisible(false);
	this->addChild(m_pShopItemPrice, 5);

	m_pShopShowCoin = Sprite::create("shop_show_coin.png");
	m_pShopShowCoin->setPosition(Vec2(m_pShopItemPrice->getPosition().x - 60, m_pShopItemPrice->getPosition().y));
	m_pShopShowCoin->setVisible(false);
	m_pShopShowCoin->setScale(2.0f);
	this->addChild(m_pShopShowCoin, 5);

}
void LobbyScene::InitSprite()
{
	m_pBackGround = Sprite::create("lobby_bg.png");
	ResManager::GetInstance()->SetFullSizeSprite(m_pBackGround);
	this->addChild(m_pBackGround, 0);
	//
	//
	m_pUICash = Sprite::createWithSpriteFrameName("ui_cash.png");
	m_pUICash->setPosition(Vec2(ResManager::GetInstance()->GetDisplaySize().width / 4, ResManager::GetInstance()->GetDisplaySize().height - 30));
	this->addChild(m_pUICash, 1);

	m_pUIMoney = Sprite::createWithSpriteFrameName("ui_money.png");
	m_pUIMoney->setPosition(Vec2(ResManager::GetInstance()->GetDisplaySize().width / 2, ResManager::GetInstance()->GetDisplaySize().height - 30));
	this->addChild(m_pUIMoney, 1);

	m_pUIStamina = Sprite::createWithSpriteFrameName("ui_stamina.png");
	m_pUIStamina->setPosition(Vec2(ResManager::GetInstance()->GetDisplaySize().width - 150, ResManager::GetInstance()->GetDisplaySize().height - 30));
	this->addChild(m_pUIStamina, 1);
	//
	//
	Sprite* pReady00 = CCSprite::createWithSpriteFrameName("ready_button.png");
	MenuItemSprite* pReadyButton00 = MenuItemSprite::create(pReady00, pReady00, CC_CALLBACK_1(LobbyScene::SelectGameState, this));
	pReadyButton00->setName("Ready");
	m_vcSelectStateButton.pushBack(pReadyButton00);

	Sprite* pReady01 = CCSprite::createWithSpriteFrameName("start_button.png");
	MenuItemSprite* pReadyButton01 = MenuItemSprite::create(pReady01, pReady01, CC_CALLBACK_1(LobbyScene::SelectGameState, this));
	pReadyButton01->setName("Start");
	pReadyButton01->setVisible(false);
	m_vcSelectStateButton.pushBack(pReadyButton01);
	//
	m_pStateButton = pReadyButton00;
	m_GameButton = Menu::create(pReadyButton00, pReadyButton01, NULL);
	m_GameButton->alignItemsVerticallyWithPadding(-70);
	m_GameButton->setPosition(Vec2(ResManager::GetInstance()->GetDisplaySize().width - 200, 80));
	this->addChild(m_GameButton);
	//
}


void LobbyScene::onMouseDown(Event* event)
{
	EventMouse* pMouse = (EventMouse*)event;
	if (m_eGameState == GAME_STATE_LOBBY)
	{
		for (auto i = m_vcSelectButton.begin(); i != m_vcSelectButton.end(); i++)
		{
			if ((*i)->getBoundingBox().containsPoint(Vec2(pMouse->getCursorX(), pMouse->getCursorY())))
			{
				SelectButton((*i));
				return;
			}
		}
	}
	else if (m_eGameState == GAME_STATE_READY)
	{
		if (m_pButtonBuy->getBoundingBox().containsPoint(Vec2(pMouse->getCursorX(), pMouse->getCursorY())))
		{
			auto pScaleBy = ScaleBy::create(0.05f, 1.05f);
			auto pReverse = pScaleBy->reverse();
			auto pSequence = Sequence::create(pScaleBy, pReverse, nullptr);
			m_pButtonBuy->runAction(pSequence);

			UserData* stUserData = GameManager::GetInstance()->GetUserData();
			if (m_pShopItem->getName() == "Health")
			{
				if (m_stUserData->iHpLevel == MAX_LEVEL)
					return;
				if (stUserData->iMoney >= stUserData->iHpLevel * HP_PRICE)
				{
					stUserData->iMoney -= stUserData->iHpLevel * HP_PRICE;
					stUserData->iHpLevel++;

					for (auto i = m_mapUIUpgrade.begin(); i != m_mapUIUpgrade.end(); i++)
					{
						if (i->first->getName() == "Health")
						{
							ShowShopInfo(i->first);
							break;
						}
					}
				}
			}
			else if (m_pShopItem->getName() == "Jelly")
			{
				if (m_stUserData->iJellyLevel == MAX_LEVEL)
					return;
				if (stUserData->iMoney >= stUserData->iJellyLevel * JELLY_PRICE)
				{
					stUserData->iMoney -= stUserData->iJellyLevel * JELLY_PRICE;
					stUserData->iJellyLevel++;

					GameManager::GetInstance()->Init(stUserData);

					for (auto i = m_mapUIUpgrade.begin(); i != m_mapUIUpgrade.end(); i++)
					{
						if (i->first->getName() == "Jelly")
						{
							ShowShopInfo(i->first);
							break;
						}
					}
				}
			}
			else if (m_pShopItem->getName() == "Time")
			{
				
			}
			char str[100] = { 0 };
			sprintf(str, "%d", GameManager::GetInstance()->GetUserData()->iMoney);
			m_pMoneyLabel->setString(str);

			return;
		}
		if (m_pBackButton->getBoundingBox().containsPoint(Vec2(pMouse->getCursorX(), pMouse->getCursorY())))
		{
			for (auto i = m_vcSelectStateButton.begin(); i != m_vcSelectStateButton.end(); i++)
			{
				if ((*i)->getName() == "Ready")
				{
					m_pStateButton = (*i);
					ChangeUI(GAME_STATE_LOBBY);
					return;
				}
			}
		}
		for (auto i = m_mapUIUpgrade.begin(); i != m_mapUIUpgrade.end(); i++)
		{
			if (i->second->getBoundingBox().containsPoint(Vec2(pMouse->getCursorX(), pMouse->getCursorY())))
			{
				m_pSelectIcon->setPosition(i->second->getPosition());
				m_pShopItem->setDisplayFrame(i->first->getDisplayFrame());
				m_pShopItem->setName((*i).first->getName());

				ShowShopInfo(i->first);
				
				return;
			}
		}
		for (auto i = m_mapUIItem.begin(); i != m_mapUIItem.end(); i++)
		{
			if (i->second->getBoundingBox().containsPoint(Vec2(pMouse->getCursorX(), pMouse->getCursorY())))
			{
				m_pSelectIcon->setPosition(i->second->getPosition());
				m_pShopItem->setDisplayFrame(i->first->getDisplayFrame());
				m_pShopItem->setName((*i).first->getName());

				ShowShopInfo(i->first);
				return;
			}
		}
	}
}

void LobbyScene::ShowShopInfo(Sprite* pShopSprite)
{
	char strSprite[100] = { 0 };
	char str[100] = { 0 };
	char strText[100] = { 0 };
	char strPrice[100] = { 0 };

	if (pShopSprite->getName() == "Health")
	{
		sprintf(strSprite, "shop_hp_%02d.png", GameManager::GetInstance()->GetUserData()->iHpLevel - 1);
		m_pShopItem->setDisplayFrame(m_pShopItemCache->getSpriteFrameByName(strSprite));
		pShopSprite->setDisplayFrame(m_pShopItemCache->getSpriteFrameByName(strSprite));
		sprintf(str, "체력 강화 Lv.%d", GameManager::GetInstance()->GetUserData()->iHpLevel);
		sprintf(strText, "추가 체력이 %d 늘어납니다.", GameManager::GetInstance()->GetUserData()->iHpLevel * 30);
		sprintf(strPrice, "%d", GameManager::GetInstance()->GetUserData()->iHpLevel * HP_PRICE);
	}
	else if (pShopSprite->getName() == "Jelly")
	{
		sprintf(strSprite, "shop_jelly_%02d.png", GameManager::GetInstance()->GetUserData()->iJellyLevel - 1);
		m_pShopItem->setDisplayFrame(m_pShopItemCache->getSpriteFrameByName(strSprite));
		pShopSprite->setDisplayFrame(m_pShopItemCache->getSpriteFrameByName(strSprite));
		sprintf(str, "젤리 강화 Lv.%d", GameManager::GetInstance()->GetUserData()->iJellyLevel);
		sprintf(strText, "젤리 점수의 획득량이 늘어납니다.");
		sprintf(strPrice, "%d", GameManager::GetInstance()->GetUserData()->iJellyLevel * JELLY_PRICE);
	}
	else if (pShopSprite->getName() == "Time")
	{
		sprintf(str, "보너스 강화 Lv.%d", GameManager::GetInstance()->GetUserData()->iTimeLevel);
		sprintf(strText, "보너스 타임의 시간이 늘어납니다.");
		sprintf(strPrice, "미구현");
	}
	else if (pShopSprite->getName() == "Hp")
	{
		sprintf(str, "체력 강화");
		sprintf(strText, "시작 체력이 증가합니다.");
		sprintf(strPrice, "미구현");
	}
	else if (pShopSprite->getName() == "Time")
	{
		sprintf(str, "아이템 시간 연장");
		sprintf(strText, "아이템의 지속시간이 증가합니다.");
		sprintf(strPrice, "미구현");
	}
	else if (pShopSprite->getName() == "Ext")
	{
		sprintf(str, "경험치 두배");
		sprintf(strText, "종료 시 획득 경험치를 2배로 얻습니다.");
		sprintf(strPrice, "미구현");
	}
	else if (pShopSprite->getName() == "Boost")
	{
		sprintf(str, "시작 부스터");
		sprintf(strText, "시작 시 부스터를 사용합니다.");
		sprintf(strPrice, "미구현");
	}
	else if (pShopSprite->getName() == "Relay")
	{
		sprintf(str, "이어달리기");
		sprintf(strText, "사용하면 설정된 쿠키로 이어달립니다.");
		sprintf(strPrice, "미구현");
	}
	else if (pShopSprite->getName() == "Box")
	{
		sprintf(str, "랜덤 아이템");
		sprintf(strText, "사용 시 랜덤으로 아이템을 얻습니다.");
		sprintf(strPrice, "미구현");
	}
	m_pShopItemName->setString(str);
	m_pShopItemText->setString(strText);
	m_pShopItemPrice->setString(strPrice);
}

void LobbyScene::SelectButton(Sprite* pSprite)
{
	auto pScaleBy = ScaleBy::create(0.05f, 1.05f);
	auto pReverse = pScaleBy->reverse();
	auto pSequence = Sequence::create(pScaleBy, pReverse, nullptr);
	pSprite->runAction(pSequence);
	m_pMainCharacter->setDisplayFrame(m_pCharacterCache->getSpriteFrameByName(pSprite->getName()));
	m_stUserData->iCharacter = pSprite->getTag();
}
void LobbyScene::SelectGameState(Ref* pSender)
{
	auto pScaleBy = ScaleBy::create(0.05f, 1.05f);
	auto pReverse = pScaleBy->reverse();
	auto pSequence = Sequence::create(pScaleBy, pReverse, nullptr);

	m_pStateButton->runAction(pSequence);
	
	if (m_pStateButton->getName() == "Ready")
	{
		for (auto i = m_vcSelectStateButton.begin(); i != m_vcSelectStateButton.end(); i++)
		{
			if ((*i)->getName() == "Start")
			{
				m_pStateButton = (*i);
				ChangeUI(GAME_STATE_READY);
			}
		}
	}
	else if (m_pStateButton->getName() == "Start")
	{
		GameManager::GetInstance()->Init(m_stUserData);
		Director::getInstance()->replaceScene(GameScene::create());
		return;
	}
}
void LobbyScene::ChangeUI(GAME_STATE eGameState)
{
	m_eGameState = eGameState;

	bool bLobbyVisible = false;
	bool bReadyVisible = false;

	if (m_eGameState == GAME_STATE_LOBBY)
	{
		bLobbyVisible = true;
		bReadyVisible = false;

	}
	else if (m_eGameState == GAME_STATE_READY)
	{
		bLobbyVisible = false;
		bReadyVisible = true;
	}
	SetVisibleLobbyObject(bLobbyVisible);
	SetVisibleShopObject(bReadyVisible);
	for (auto i = m_vcSelectStateButton.begin(); i != m_vcSelectStateButton.end(); i++)
	{
		if ((*i)->getName() == "Ready")
			(*i)->setVisible(bLobbyVisible);
		else if ((*i)->getName() == "Start")
			(*i)->setVisible(bReadyVisible);
	}
}
void LobbyScene::SetVisibleLobbyObject(bool bVisible)
{
	m_pCharacterBoard->setVisible(bVisible);
	for (auto i = m_vcSelectButton.begin(); i != m_vcSelectButton.end(); i++)
	{
		(*i)->setVisible(bVisible);
	}
	for (auto i = m_vcCharacter.begin(); i != m_vcCharacter.end(); i++)
	{
		(*i)->setVisible(bVisible);
	}
}
void LobbyScene::SetVisibleShopObject(bool bVisible)
{
	m_pUIShop->setVisible(bVisible);
	m_pUIShopItemInfo->setVisible(bVisible);
	m_pSelectIcon->setVisible(bVisible);
	m_pBackButton->setVisible(bVisible);
	m_pShopItemIcon->setVisible(bVisible);
	m_pShopItem->setVisible(bVisible);
	m_pShopItemName->setVisible(bVisible);
	m_pShopItemText->setVisible(bVisible);
	m_pButtonBuy->setVisible(bVisible);
	m_pShopItemPrice->setVisible(bVisible);
	m_pShopShowCoin->setVisible(bVisible);
	for (auto i = m_mapUIUpgrade.begin(); i != m_mapUIUpgrade.end(); i++)
	{
		i->second->setVisible(bVisible);
		i->first->setVisible(bVisible);
	}
	for (auto i = m_mapUIItem.begin(); i != m_mapUIItem.end(); i++)
	{
		i->second->setVisible(bVisible);
		i->first->setVisible(bVisible);
	}
}