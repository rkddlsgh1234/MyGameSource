#include "GameScene.h"
#include "LobbyScene.h"

#include "GameManager.h"
#include "ResManager.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	delete m_pObjectManager;
	delete m_pPlayer;
	m_pPlayer = NULL;
}

Scene* GameScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameScene::create();
	scene->addChild(layer);

	return scene;
}

bool GameScene::init()
{
	m_iResultCoin = 0;
	m_iResultJelly = 0;

	auto Mouse_Listener = EventListenerMouse::create();
	Mouse_Listener->onMouseDown = CC_CALLBACK_1(GameScene::onMouseDown, this);
	Mouse_Listener->onMouseUp = CC_CALLBACK_1(GameScene::onMouseUp, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(Mouse_Listener, this);

	m_pObjectManager = new ObjectManager;

	m_bMousePush = false;
	m_bGameStart = false;
	m_bFever = false;
	m_bResult = false;
	InitGame();

	return true;
}
void GameScene::InitGame()
{
	m_bEndGame = false;

	InitCache();
	InitPlayer();
	m_pObjectManager->Init(this);
	GameManager::GetInstance()->ResetScore();
	this->schedule(schedule_selector(GameScene::StartGame, this), 3.0f);

	
}
void GameScene::InitPlayer()
{
	char str[100] = { 0 };

	m_pCharacterAni = SpriteFrameCache::sharedSpriteFrameCache();
	if (GameManager::GetInstance()->GetUserData()->iCharacter == 0)
	{
		m_pPlayer = new PlayerCookie;
		m_pCharacterAni->addSpriteFramesWithFile("character_00.plist");
	}
	else if(GameManager::GetInstance()->GetUserData()->iCharacter == 1)
	{
		m_pPlayer = new PlayerCookie;
		m_pCharacterAni->addSpriteFramesWithFile("character_01.plist");
	}
	else if (GameManager::GetInstance()->GetUserData()->iCharacter == 2)
	{
		m_pPlayer = new OrangeCookie;
		m_pCharacterAni->addSpriteFramesWithFile("character_02.plist");
	}
	else if (GameManager::GetInstance()->GetUserData()->iCharacter == 3)
	{
		m_pPlayer = new PeachCookie;
		m_pCharacterAni->addSpriteFramesWithFile("character_03.plist");
	}

	m_pPlayer->Init(this, m_pCharacterAni, m_pObjectManager);
	m_pObjectManager->InitPlayer(m_pPlayer->GetSprite());

	m_pJumpButton = Sprite::createWithSpriteFrameName("play_jump_button.png");
	m_pJumpButton->setPosition(Vec2(m_pJumpButton->getContentSize().width / 2 + 20, 70));
	m_pJumpButton->setVisible(false);
	this->addChild(m_pJumpButton, 20);

	m_pSlideButton = Sprite::createWithSpriteFrameName("play_slide_button.png");
	m_pSlideButton->setPosition(Vec2(ResManager::GetInstance()->GetDisplaySize().width - (m_pSlideButton->getContentSize().width / 2) - 20, 70));
	m_pSlideButton->setVisible(false);
	this->addChild(m_pSlideButton, 20);

	for (int i = 0; i < 9; i++)
	{
		sprintf(str, "play_bonus_empty_%02d.png", i);
		Sprite* pBounsTime = Sprite::createWithSpriteFrameName(str);
		sprintf(str, "play_bonus_%02d.png", i);
		pBounsTime->setName(str);
		pBounsTime->setTag(0);
		pBounsTime->setPosition(Vec2(15 + (i * 27), ResManager::GetInstance()->GetDisplaySize().height - 20));
		this->addChild(pBounsTime, 1);
		m_vcBounsTime.pushBack(pBounsTime);
	}

	m_pBonus = Sprite::create("play_bonustime_00.png");
	m_pBonus->setAnchorPoint(Vec2(0, 0));
	m_pBonus->setPosition(Vec2(3 , ResManager::GetInstance()->GetDisplaySize().height - 37));
	m_pBonus->setVisible(false);
	this->addChild(m_pBonus, 1);

	m_pEffectHit = Sprite::create("effect_hit.png");
	m_pEffectHit->setOpacity(0);
	ResManager::GetInstance()->SetFullSizeSprite(m_pEffectHit);
	m_pEffectHit->setVisible(false);
	this->addChild(m_pEffectHit, 30);

	m_pEffectHealthHit = Sprite::create("effect_hit.png");
	m_pEffectHealthHit->setOpacity(0);
	m_pEffectHealthHit->setVisible(false);
	ResManager::GetInstance()->SetFullSizeSprite(m_pEffectHealthHit);
	this->addChild(m_pEffectHealthHit, 30);

	FadeIn* pFadeIn = FadeIn::create(0.3f);
	FadeOut* pFadeOut = FadeOut::create(0.3f);
	Sequence* pSequence = Sequence::create(pFadeIn, pFadeOut, nullptr);
	RepeatForever* pRepeat = RepeatForever::create(pSequence);
	m_pEffectHealthHit->runAction(pRepeat);
	
	m_pGetCoin = Sprite::createWithSpriteFrameName("result_board_coin.png");
	m_pGetCoin->setPosition(Vec2(ResManager::GetInstance()->GetDisplaySize().width / 2 - 30, ResManager::GetInstance()->GetDisplaySize().height - 20));
	m_pGetCoin->setScale(1.5);
	m_pGetCoin->setVisible(false);
	this->addChild(m_pGetCoin, 1);

	m_pGetCoinLabel = Label::createWithTTF("0", "fonts/Anton.ttf", 25);
	m_pGetCoinLabel->setPosition(Vec2(m_pGetCoin->getPosition().x + 50, m_pGetCoin->getPosition().y));
	m_pGetCoinLabel->setVisible(false);
	this->addChild(m_pGetCoinLabel, 1);

	sprintf(str, "shop_jelly_%02d.png", GameManager::GetInstance()->GetUserData()->iJellyLevel - 1);
	m_pGetJelly = Sprite::createWithSpriteFrameName(str);
	m_pGetJelly->setPosition(Vec2(ResManager::GetInstance()->GetDisplaySize().width - 100, ResManager::GetInstance()->GetDisplaySize().height - 30));
	m_pGetJelly->setScale(1.5);
	m_pGetJelly->setVisible(false);
	this->addChild(m_pGetJelly, 1);

	m_pGetJellyLabel = Label::createWithTTF("0", "fonts/Anton.ttf", 25);
	m_pGetJellyLabel->setPosition(Vec2(ResManager::GetInstance()->GetDisplaySize().width - 65, ResManager::GetInstance()->GetDisplaySize().height - 30));
	m_pGetJellyLabel->setVisible(false);
	this->addChild(m_pGetJellyLabel, 1);
	

	float fHpMax = 300 + (GameManager::GetInstance()->GetUserData()->iHpLevel * 30);
	Rect rcHealth = { 0, 0, fHpMax, 24 };

	m_pHealthEmpty = Sprite::create("play_health_empty.png");
	m_pHealthEmpty->setAnchorPoint(Vec2(0, 0));
	m_pHealthEmpty->setTextureRect(rcHealth);
	m_pHealthEmpty->setPosition(Vec2(40, ResManager::GetInstance()->GetDisplaySize().height - 70));
	m_pHealthEmpty->setVisible(false);
	this->addChild(m_pHealthEmpty, 1);
	m_pHealth = Sprite::create("play_health.png");
	m_pHealth->setTextureRect(rcHealth);
	m_pHealth->setAnchorPoint(Vec2(0, 0));
	m_pHealth->setPosition(Vec2(40, ResManager::GetInstance()->GetDisplaySize().height - 70));
	m_pHealth->setVisible(false);
	this->addChild(m_pHealth, 1);

	m_pHealthShowMax = Sprite::createWithSpriteFrameName("play_health_max_00.png");
	m_pHealthShowMax->setPosition(m_pHealth->getBoundingBox().getMaxX(), m_pHealth->getBoundingBox().getMaxY() - 12);
	m_pHealthShowMax->setVisible(false);
	this->addChild(m_pHealthShowMax, 1);

	Vector<SpriteFrame*> pAniFrames;
	
	for (int i = 0; i < 2; i++)
	{
		sprintf(str, "play_health_max_%02d.png", i);
		SpriteFrame* Frame = m_pPlayingCache->getSpriteFrameByName(str);
		pAniFrames.pushBack(Frame);
	}
	Animation* pAnimation = Animation::createWithSpriteFrames(pAniFrames, 0.1f);
	Animate* pAnimate = Animate::create(pAnimation);
	RepeatForever* pRepeatForever = RepeatForever::create(pAnimate);
	m_pHealthShowMax->runAction(pRepeatForever);

	sprintf(str, "shop_hp_%02d.png", GameManager::GetInstance()->GetUserData()->iHpLevel - 1);
	m_pHealthIcon = Sprite::createWithSpriteFrameName(str);
	m_pHealthIcon->setScale(1.5f);
	m_pHealthIcon->setPosition(Vec2(m_pHealthEmpty->getPosition().x + 5, m_pHealthEmpty->getPosition().y + 5));
	m_pHealthIcon->setVisible(false);
	this->addChild(m_pHealthIcon, 1);

	m_pResultBoard = Sprite::createWithSpriteFrameName("result_score_board.png");
	m_pResultBoard->setPosition(Vec2(ResManager::GetInstance()->GetDisplaySize().width / 2, ResManager::GetInstance()->GetDisplaySize().height / 2));
	m_pResultBoard->setScale(2.0f);
	m_pResultBoard->setVisible(false);
	this->addChild(m_pResultBoard, 10);
	m_pResultTitle = Sprite::createWithSpriteFrameName("result_score_star.png");
	m_pResultTitle->setPosition(Vec2(m_pResultBoard->getPosition().x, m_pResultBoard->getBoundingBox().getMaxY()));
	m_pResultTitle->setScale(0);
	m_pResultTitle->setVisible(false);
	this->addChild(m_pResultTitle, 11);

	sprintf(str, "shop_jelly_%02d.png", GameManager::GetInstance()->GetUserData()->iJellyLevel - 1);
	m_pResultJelly = Sprite::createWithSpriteFrameName(str);
	m_pResultJelly->setPosition(Vec2(m_pResultBoard->getPosition().x - 60, m_pResultBoard->getBoundingBox().getMidY() - 25));
	m_pResultJelly->setScale(2.0f);
	m_pResultJelly->setVisible(false);
	this->addChild(m_pResultJelly, 11);
	m_pResultCoin = Sprite::createWithSpriteFrameName("result_board_coin.png");
	m_pResultCoin->setPosition(Vec2(m_pResultBoard->getPosition().x - 60, m_pResultBoard->getBoundingBox().getMidY() + 25));
	m_pResultCoin->setScale(2.0f);
	m_pResultCoin->setVisible(false);
	this->addChild(m_pResultCoin, 11);

	m_pResultCoinLabel = Label::createWithTTF("0", "fonts/Anton.ttf", 25);
	m_pResultCoinLabel->setColor(Color3B(0, 0, 0));
	m_pResultCoinLabel->setPosition(Vec2(m_pResultCoin->getPosition().x + 70, m_pResultCoin->getPosition().y));
	m_pResultCoinLabel->setVisible(false);
	this->addChild(m_pResultCoinLabel, 11);

	m_pResultJellyLabel = Label::createWithTTF("0", "fonts/Anton.ttf", 25);
	m_pResultJellyLabel->setColor(Color3B(0, 0, 0));
	m_pResultJellyLabel->setPosition(Vec2(m_pResultJelly->getPosition().x + 70, m_pResultJelly->getPosition().y));
	m_pResultJellyLabel->setVisible(false);
	this->addChild(m_pResultJellyLabel, 11);

	m_pResultExitButton = Sprite::create("exit_game_button.png");
	m_pResultExitButton->setPosition(Vec2(m_pResultBoard->getPosition().x, m_pResultBoard->getBoundingBox().getMinY()));
	m_pResultExitButton->setVisible(false);
	m_pResultExitButton->setScale(0);
	this->addChild(m_pResultExitButton, 11);
	
}
void GameScene::InitCache()
{

	m_pPlayingCache = SpriteFrameCache::sharedSpriteFrameCache();
	m_pPlayingCache->addSpriteFramesWithFile("playing.plist");
	m_pResultCache = SpriteFrameCache::sharedSpriteFrameCache();
	m_pResultCache->addSpriteFramesWithFile("result.plist");
}

void GameScene::Updata(float DelayTime)
{
	if (m_bEndGame)
		return;

	
	if (m_pPlayer->Updata(m_pObjectManager))
	{
		m_bEndGame = true;
		m_pPlayer->SetHealthCookie();
		this->schedule(schedule_selector(GameScene::ShowScore, this), 4.0f);
		return;
	}
	m_pPlayer->UpdataCharacter();

	if (m_pHealth->getBoundingBox().getMaxX() < 120)
		m_pEffectHealthHit->setVisible(true);
	else
		m_pEffectHealthHit->setVisible(false);


	UpdataFever();
	

	
	char str[128] = { 0 };
	sprintf(str, "%d", GameManager::GetInstance()->GetCoin());
	m_pGetCoinLabel->setString(str);
	sprintf(str, "%d", GameManager::GetInstance()->GetJelly());
	m_pGetJellyLabel->setString(str);

	m_pObjectManager->MoveObject();
	m_pObjectManager->Updata();
	CheckCrash();
	CheckItem();
}
void GameScene::UpdataFever()
{
	if (m_bFever == false)
		return;

	Rect rcRect = m_pBonus->getTextureRect();
	float fRect = rcRect.getMaxX();
	fRect -= 0.3f;
	rcRect = { 0, 0, fRect, m_pBonus->getTextureRect().getMaxY() };
	m_pBonus->setTextureRect(rcRect);

	if (fRect <= 0)
		UnFeverMode();

}

void GameScene::CheckCrash()
{
	if (m_pPlayer->CheckCrash())
	{
		SetGod();
		for (int i = 0; i < 2; i++)
		{
			SetHealth();
		}

		Show* pShow = Show::create();
		FadeIn* pFadeIn = FadeIn::create(0.2f);
		FadeOut* pFadeOut = FadeOut::create(0.2f);
		Hide* pHide = Hide::create();
		DelayTime* pDelayTime = DelayTime::create(0.2f);
		Sequence* pSequence = Sequence::create(pShow, pFadeIn, pFadeOut, pHide, nullptr);
		m_pEffectHit->runAction(pSequence);
	}
}

void GameScene::CheckItem()
{
	if (m_pObjectManager->CheckGetItem(m_pPlayer->GetSprite()))
	{
		if (m_pPlayer->GetSprite()->getName() == "play_item_big.png")
		{			
			m_pPlayer->GetSprite()->setPosition(Vec2(m_pPlayer->GetSprite()->getPosition().x, 180));
			m_pPlayer->SetItem(m_pPlayer->GetSprite()->getName());	
			DelayTime* pDelayTime = DelayTime::create(4.0f);
			auto pItemCoolTime = CallFuncN::create(CC_CALLBACK_0(GameScene::SetItemBig, this));
			Sequence* pSequence = Sequence::create(pDelayTime, pItemCoolTime, nullptr);
			m_pGetCoin->runAction(pSequence);
		}
		else if (m_pPlayer->GetSprite()->getName() == "play_item_speed.png")
		{
			m_pPlayer->SetItem(m_pPlayer->GetSprite()->getName());
			m_pPlayer->ChangeGodMode();
			m_pObjectManager->ChangeMaxSpeed();
			DelayTime* pDelayTime = DelayTime::create(4.0f);
			auto pItemCoolTime = CallFuncN::create(CC_CALLBACK_0(GameScene::SetItemSpeed, this));
			Sequence* pSequence = Sequence::create(pDelayTime, pItemCoolTime, nullptr);
			m_pGetCoin->runAction(pSequence);
		}
		if (m_pPlayer->GetSprite()->getName() == "play_item_small_health.png")
			SetHealLife(25);
		if (m_pPlayer->GetSprite()->getName() == "play_item_big_health.png")
			SetHealLife(70);
	}
	if (m_pObjectManager->CheckGetJelly(m_pPlayer->GetSprite()))
	{
		if (m_pPlayer->GetSprite()->getName() == "Jelly")
			GameManager::GetInstance()->SetJellyScore(1, true);
		else if(m_pPlayer->GetSprite()->getName() == "Coin")
			GameManager::GetInstance()->SetScore(1, true);
		else
		{
			for (auto i = m_vcBounsTime.begin(); i != m_vcBounsTime.end(); i++)
			{
				if ((*i)->getName() == m_pPlayer->GetSprite()->getName())
				{
					(*i)->setDisplayFrame(m_pPlayingCache->getSpriteFrameByName((*i)->getName()));
					(*i)->setScale(0.9f);
					(*i)->setTag(1);
					CheckBonusItem();
				}
			}

			
		}

		m_pPlayer->ShowEffect();
	}
}
void GameScene::CheckBonusItem()
{
	int iCount = 0;
	for (auto i = m_vcBounsTime.begin(); i != m_vcBounsTime.end(); i++)
	{
		if ((*i)->getTag() == 1)
			iCount++;
	}
	if (iCount == 9)
	{
		char str[100] = { 0 };
		int iLen = 0;
		for (auto i = m_vcBounsTime.begin(); i != m_vcBounsTime.end(); i++)
		{
			sprintf(str, "play_bonus_empty_%02d.png", iLen);
			(*i)->setDisplayFrame(m_pPlayingCache->getSpriteFrameByName(str));
			(*i)->setScale(1.0f);
			(*i)->setTag(0);
			iLen++;
		}
		m_pPlayer->SetFeverMode();
		m_pObjectManager->ChangeFeverMode();
		m_bFever = true;
		m_pBonus->setVisible(true);
	}
}

void GameScene::SetGodMode()
{
	m_pPlayer->GetSprite()->setOpacity(255);
	m_pPlayer->ChangeGodMode();
}
void GameScene::SetItemBig()
{
	m_pPlayer->ResetItem("play_item_big.png");
	SetGod();
}
void GameScene::SetItemSpeed()
{
	m_pPlayer->ResetItem("play_item_speed.png");
	m_pPlayer->ChangeGodMode();
	m_pObjectManager->ChangeMinSpeed();
	SetGod();
}
void GameScene::SetGod()
{
	m_pPlayer->ChangeGodMode();
	m_pPlayer->GetSprite()->setOpacity(130);
	DelayTime* pDelayTime = DelayTime::create(2.0f);
	auto pItemCoolTime = CallFuncN::create(CC_CALLBACK_0(GameScene::SetGodMode, this));
	Sequence* pSequence = Sequence::create(pDelayTime, pItemCoolTime, nullptr);
	m_pGetCoin->runAction(pSequence);
}

void GameScene::UpdataDelay(float DelayTime)
{
	if (m_bEndGame)
		return;

	SetHealth();
}
void GameScene::ChangePlayerGodMode(float DelayTime)
{
	this->unschedule(schedule_selector(GameScene::ChangePlayerGodMode, this));
	m_pPlayer->ChangeGodMode();
}

void GameScene::SetHealLife(int Heal)
{
	Rect rcHealth = m_pHealth->getTextureRect();
	float fHealth = rcHealth.getMaxX();
	fHealth += Heal;

	rcHealth = { 0, 0, fHealth, 24 };

	m_pHealth->setTextureRect(rcHealth);
	m_pHealthShowMax->setPosition(m_pHealth->getBoundingBox().getMaxX(), m_pHealth->getBoundingBox().getMaxY() - 12);
}
bool GameScene::SetHealth()
{
	if (m_bFever)
		return false;

	Rect rcHealth = m_pHealth->getTextureRect();
	float fHealth = rcHealth.getMaxX();
	fHealth -= 7;

	if (fHealth <= 0)
	{
		m_pEffectHealthHit->setVisible(false);
		m_bEndGame = true;
		m_pPlayer->GetSprite()->setOpacity(255);
		m_pPlayer->GetSprite()->setScale(2.0f);
		m_pPlayer->SetHealthCookie();
		this->schedule(schedule_selector(GameScene::ShowScore, this), 4.0f);
	}
	
	rcHealth = { 0, 0, fHealth, 24 };

	m_pHealth->setTextureRect(rcHealth);
	m_pHealthShowMax->setPosition(m_pHealth->getBoundingBox().getMaxX(), m_pHealth->getBoundingBox().getMaxY() - 12);

	return false;
}

void GameScene::ShowScore(float DelayTime)
{
	this->unschedule(schedule_selector(GameScene::ShowScore, this));
	this->schedule(schedule_selector(GameScene::ResultUpdata, this));
	m_pResultBoard->setVisible(true);
	m_pResultTitle->setVisible(true);
	m_pResultJelly->setVisible(true);
	m_pResultCoin->setVisible(true);
	m_pResultCoinLabel->setVisible(true);
	m_pResultJellyLabel->setVisible(true);
	m_bResult = true;
	
	ScaleTo* pScaleTo = ScaleTo::create(1.0f, 2.0f);
	m_pResultTitle->runAction(pScaleTo);
}
void GameScene::EndScene()
{
	this->unschedule(schedule_selector(GameScene::ResultUpdata, this));
	m_pResultExitButton->setVisible(true);
	ScaleTo* pScaleTo = ScaleTo::create(1.0f, 2.0f);
	m_pResultExitButton->runAction(pScaleTo);
	m_bResult = false;

	char str[100] = { 0 };
	m_iResultCoin += GameManager::GetInstance()->GetCoin();
	m_iResultCoin += GameManager::GetInstance()->GetJelly() * 10;
	sprintf(str, "%d", m_iResultCoin);
	m_pResultCoinLabel->setString(str);
	sprintf(str, "%d", 0);
	m_pResultJellyLabel->setString(str);
}
void GameScene::ResultUpdata(float DelayTime)
{
	if (GameManager::GetInstance()->GetCoin() == 0 && GameManager::GetInstance()->GetJelly() == 0)
	{
		m_bResult = false;
		m_pResultExitButton->setVisible(true);
		ScaleTo* pScaleTo = ScaleTo::create(1.0f, 2.0f);
		m_pResultExitButton->runAction(pScaleTo);
		this->unschedule(schedule_selector(GameScene::ResultUpdata, this));
	}
	if(GameManager::GetInstance()->GetCoin() != 0)
	{
		GameManager::GetInstance()->SetScore(1, false);
		m_iResultCoin += 10;
	}
	if (GameManager::GetInstance()->GetJelly() != 0)
	{
		GameManager::GetInstance()->SetJellyScore(1, false);
		m_iResultCoin += 10;
	}

	char str[100] = { 0 };
	
	sprintf(str, "%d", m_iResultCoin);
	m_pResultCoinLabel->setString(str);
	sprintf(str, "%d", GameManager::GetInstance()->GetJelly());
	m_pResultJellyLabel->setString(str);
}

void GameScene::ReadyUpdata(float DelayTime)
{
	if (m_pObjectManager->GetGameState())
	{
		m_bGameStart = true;
		this->unschedule(schedule_selector(GameScene::StartGame, this));
		this->unschedule(schedule_selector(GameScene::ReadyUpdata, this));
		this->schedule(schedule_selector(GameScene::Updata, this));
		this->schedule(schedule_selector(GameScene::UpdataDelay, this), 1.0f);
		m_pPlayer->StartGame();
		m_pJumpButton->setVisible(true);
		m_pSlideButton->setVisible(true);
		m_pHealth->setVisible(true);
		m_pHealthEmpty->setVisible(true);
		m_pHealthIcon->setVisible(true);
		m_pHealthShowMax->setVisible(true);
		m_pGetCoin->setVisible(true);
		m_pGetCoinLabel->setVisible(true);
		m_pGetJelly->setVisible(true);
		m_pGetJellyLabel->setVisible(true);

		for (auto i = m_vcBounsTime.begin(); i != m_vcBounsTime.end(); i++)
		{
			(*i)->setVisible(true);
		}
	}
}

void GameScene::StartGame(float DelayTime)
{
	m_pObjectManager->InitStart();
	this->schedule(schedule_selector(GameScene::ReadyUpdata, this));
}

void GameScene::onMouseDown(Event* event)
{
	EventMouse* pMouse = (EventMouse*)event;

	if (m_bResult)
		EndScene();

	if (m_bEndGame)
	{
		if (m_pResultExitButton->getBoundingBox().containsPoint(Vec2(pMouse->getCursorX(), pMouse->getCursorY())))
		{
			GameManager::GetInstance()->GetUserData()->iMoney += m_iResultCoin;
			Director::getInstance()->replaceScene(LobbyScene::create());
		}
	}

	if (m_bGameStart == false)
		return;

	if (pMouse->getCursorX() < ResManager::GetInstance()->GetDisplaySize().width / 2)
	{
		m_pJumpButton->setDisplayFrame(m_pPlayingCache->getSpriteFrameByName("play_jump_button_push.png"));
		if (m_bFever)
		{
			m_bMousePush = true;
			m_pPlayer->InputFever();
		}
		else
			m_pPlayer->Input();		
	}
	else
	{
		m_pSlideButton->setDisplayFrame(m_pPlayingCache->getSpriteFrameByName("play_slide_button_push.png"));
		m_pPlayer->InputPush();
		m_bMousePush = true;
	}
}
void GameScene::onMouseUp(Event* event)
{
	EventMouse* pMouse = (EventMouse*)event;

	m_bMousePush = false;

	if (pMouse->getCursorX() < ResManager::GetInstance()->GetDisplaySize().width / 2)
	{
		m_pJumpButton->setDisplayFrame(m_pPlayingCache->getSpriteFrameByName("play_jump_button.png"));
		m_pPlayer->UnPush();
	}
	else
	{
		m_pSlideButton->setDisplayFrame(m_pPlayingCache->getSpriteFrameByName("play_slide_button.png"));
		if (m_bFever)
			return;
		if (m_pPlayer->GetSkil())
			m_pPlayer->SetRunAni();
		else
		{
			if (m_pPlayer->GetJumpState())
				m_pPlayer->SetRunAnimate();
		}
		
	}
}

void GameScene::UnFeverMode()
{
	m_pBonus = Sprite::create("play_bonustime_00.png");
	m_pBonus->setAnchorPoint(Vec2(0, 0));
	m_pBonus->setPosition(Vec2(3, ResManager::GetInstance()->GetDisplaySize().height - 37));
	m_pBonus->setVisible(false);

	m_bFever = false;
	m_pObjectManager->UnChangeFeverMode();
	m_pPlayer->UnFeverMode();
	SetGod();
}