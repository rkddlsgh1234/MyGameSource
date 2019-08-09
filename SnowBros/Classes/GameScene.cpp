#include "GameScene.h"
#include "ResManager.h"
#include "SimpleAudioEngine.h"
#include "TitleScene.h"

using namespace CocosDenshion;

#include "RedMob.h"

USING_NS_CC;

GameScene::GameScene()
{
}


GameScene::~GameScene()
{
	m_pPlayer->release();
	m_pBoss->release();
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
	auto KeyBoard_Listener = EventListenerKeyboard::create();
	KeyBoard_Listener->onKeyPressed = CC_CALLBACK_2(GameScene::PressKeyboard, this);
	KeyBoard_Listener->onKeyReleased = CC_CALLBACK_2(GameScene::ReleaseKeyboard, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(KeyBoard_Listener, this);

	InitCache();
	InitSprite();

	m_bStart = true;
	m_bGameOver = false;
	m_bKeyPush = false;
	m_bEnd = false;
	m_bEndRound = false;
	m_iCombo = 0;
	m_iScore = 0;
	m_iLife = 3;
	m_iLife--;
	m_eGameStage = GAME_STAGE_ONE;
	m_pPlayer = Playerble::create();
	m_pPlayer->retain();
	m_pPlayer->Init(this);

	m_pUi_Player = Sprite::createWithSpriteFrameName("ui_game_1p.png");
	m_pUi_Player->setPosition(Vec2(35, ResManager::GetInstance()->GetDisplaySize().height - 20));
	this->addChild(m_pUi_Player, 30);

	m_pUi_ScoreLabel = Label::createWithTTF("0", "fonts/Sabo-Filled.otf", 42);
	m_pUi_ScoreLabel->setPosition(Vec2(150, ResManager::GetInstance()->GetDisplaySize().height - 20));
	this->addChild(m_pUi_ScoreLabel, 30);

	m_pUi_LifeIcon = Sprite::createWithSpriteFrameName("ui_game_cha.png");
	m_pUi_LifeIcon->setPosition(Vec2(ResManager::GetInstance()->GetDisplaySize().width / 2 + 50, ResManager::GetInstance()->GetDisplaySize().height - 23));
	this->addChild(m_pUi_LifeIcon, 30);

	m_pUi_LifeLabe = Label::createWithTTF("2", "fonts/Sabo-Filled.otf", 42);
	m_pUi_LifeLabe->setPosition(Vec2(m_pUi_LifeIcon->getPosition().x + 35, m_pUi_LifeIcon->getPosition().y));
	this->addChild(m_pUi_LifeLabe, 30);

	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("sound/bgm_b_s_01.mp3");
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("sound/bgm_b_boss_01.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/fx_gameover_00_01.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/fx_stage_boss_01.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/fx_stage_next_01.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/fx_start_play_01.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/fx_item_00_01.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/fx_jump_00_01.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/fx_laser_00_01.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/fx_death_00_01.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/fx_enemy_kill_01.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/fx_boss_body_01.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/fx_boss_death_01.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("sound/fx_snow_hit_01.wav");
	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.0f);

	StartGame();

	this->schedule(schedule_selector(GameScene::Updata, this));

	auto pDelay = DelayTime::create(1.3f);
	auto pStart = CallFuncN::create(CC_CALLBACK_0(GameScene::StartSound, this));
	auto pSequenceTwo = Sequence::create(pDelay, pStart, nullptr);
	m_pStageBg->runAction(pSequenceTwo);

	return true;
}

void GameScene::StartGame() // 게임 시작
{
	char str[100] = { 0 };
	Vector<SpriteFrame*> pAniFramesOne;
	Vector<SpriteFrame*> pAniFramesTwo;
	for (int i = 0; i < 5; i++)
	{
		sprintf(str, "effect_spawn_%02d.png", i);
		SpriteFrame* Frame = m_pEffectCache->getSpriteFrameByName(str);
		pAniFramesOne.pushBack(Frame);
	}
	for (int i = 5; i < 7; i++)
	{
		sprintf(str, "effect_spawn_%02d.png", i);
		SpriteFrame* Frame = m_pEffectCache->getSpriteFrameByName(str);
		pAniFramesTwo.pushBack(Frame);
	}
	
	Sprite* pEffectSpawn = Sprite::createWithSpriteFrameName("effect_spawn_00.png");
	pEffectSpawn->setAnchorPoint(Vec2(0.5f, 0));
	pEffectSpawn->setPosition(m_pPlayer->GetSprite()->getPosition().x, m_pPlayer->GetSprite()->getPosition().y - 40);
	Animation* pAnimationOne = Animation::createWithSpriteFrames(pAniFramesOne, 0.12f);
	Animation* pAnimationTwo = Animation::createWithSpriteFrames(pAniFramesTwo, 0.12f);
	Animate* pAnimateOne = Animate::create(pAnimationOne);
	Animate* pAnimateTwo = Animate::create(pAnimationTwo);
	auto pSpawn = CallFuncN::create(CC_CALLBACK_0(GameScene::SpawnPlayer, this));
	auto pHide = Hide::create();
	auto pSequence = Sequence::create(pAnimateOne, pSpawn, pAnimateTwo, pHide, nullptr);
	pEffectSpawn->runAction(pSequence);

	this->addChild(pEffectSpawn);

	SimpleAudioEngine::getInstance()->playEffect("sound/fx_start_play_01.wav");
	
}
void GameScene::StartSound() // 게임 배경음악
{
	SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/bgm_b_s_01.mp3", true);
}

void GameScene::SpawnPlayer() // 플레이어 스폰
{
	m_pPlayer->SetLife();
	m_pPlayer->Start();
}

void GameScene::Updata(float DelayTime)
{
	if (m_bStart == false)
		return;
	if (m_bGameOver)
		return;

	SetGravity();
	SetGravityEnemy();
	SetGravityItem();
	SetGravityBoss();

	if (m_bKeyPush)
	{
		if (CheckCrashTile() == false)
		{
			CheckCrashPlayerToEnemy();
			m_pPlayer->MovePlayer(m_iKeyCode);
		}
	}
	MoveEnemy();

	CheckCrashBulletToEnemy();
	CheckCrashIceEnemyToEnemy();
	CheckCrashPlayerToItem();
	CheckCrashItem();
	CheckClearRound();
	CheckCrashEnemyToBoss();
	CheckCrashBossToPlayer();

	CheckDeadBoss();

	char str[10] = { 0 };
	sprintf(str, "%d", m_iScore);
	m_pUi_ScoreLabel->setString(str);
}

void GameScene::InitCache()
{
	m_pStageCache_00 = SpriteFrameCache::getInstance();
	m_pStageCache_00->addSpriteFramesWithFile("stage_00.plist");
	m_pStageCache_01 = SpriteFrameCache::getInstance();
	m_pStageCache_01->addSpriteFramesWithFile("stage_01.plist");
	m_pStageCache_02 = SpriteFrameCache::getInstance();
	m_pStageCache_02->addSpriteFramesWithFile("stage_02.plist");
	m_pBossCache_00 = SpriteFrameCache::getInstance();
	m_pBossCache_00->addSpriteFramesWithFile("boss_00.plist");
	m_pMobRedCache = SpriteFrameCache::getInstance();
	m_pMobRedCache->addSpriteFramesWithFile("mob_00.plist");
	m_pMobBeastCache = SpriteFrameCache::getInstance();
	m_pMobBeastCache->addSpriteFramesWithFile("mob_01.plist");
	m_pEffectCache = SpriteFrameCache::getInstance();
	m_pEffectCache->addSpriteFramesWithFile("ice.plist");
	m_pIceCache = SpriteFrameCache::getInstance();
	m_pIceCache->addSpriteFramesWithFile("effect.plist");
	m_pNumCache = SpriteFrameCache::getInstance();
	m_pNumCache->addSpriteFramesWithFile("num.plist");
	m_pUICache = SpriteFrameCache::getInstance();
	m_pUICache->addSpriteFramesWithFile("ui.plist");
	m_pItemCache = SpriteFrameCache::getInstance();
	m_pItemCache->addSpriteFramesWithFile("item.plist");
}
void GameScene::InitSprite()
{
	char str[100] = { 0 };
	m_pStageBg_00 = Sprite::createWithSpriteFrameName("stage_00_bg.png");
	m_pStageBg_00->setAnchorPoint(Vec2(0, 0));
	m_pStageBg_00->setPosition(Vec2(0, 0));
	this->addChild(m_pStageBg_00);

	for (int i = 0; i < 10; i++)
	{
		sprintf(str, "stage_00_%02d.png", i);
		Sprite* pTile = Sprite::createWithSpriteFrameName(str);
		pTile->setAnchorPoint(Vec2(0, 1));
		pTile->setTag(i);
		pTile->setName("Normal");

		if(i == 0)
			pTile->setPosition(0, pTile->getContentSize().height);
		else if(i == 1)
			pTile->setPosition(0.5f, 135);
		else if(i == 2)
			pTile->setPosition(248.5f, 135);
		else if (i == 3)
			pTile->setPosition(473.5f, 135);
		else if(i == 4)
			pTile->setPosition(113.5f, 225);
		else if (i == 5)
			pTile->setPosition(0, 315);
		else if (i == 6)
			pTile->setPosition(ResManager::GetInstance()->GetDisplaySize().width - pTile->getContentSize().width - 0.5f, 315);
		else if (i == 7)
			pTile->setPosition(91, 405);
		else if (i == 8)
		{
			pTile->setPosition(180, 450);
			pTile->setName("Tile");
		}
		else if (i == 9)
			pTile->setPosition(450, 405);

		m_vcStageTile_00.pushBack(pTile);
		this->addChild(pTile);
	}

	m_pStageBg_01 = Sprite::createWithSpriteFrameName("stage_01_bg.png");
	m_pStageBg_01->setAnchorPoint(Vec2(0, 0));
	m_pStageBg_01->setPosition(Vec2(0, 720.0f));
	this->addChild(m_pStageBg_01);

	for (int i = 0; i < 10; i++)
	{
		sprintf(str, "stage_01_%02d.png", i);
		Sprite* pTile = Sprite::createWithSpriteFrameName(str);
		pTile->setAnchorPoint(Vec2(0, 1));
		pTile->setTag(i);
		pTile->setName("Normal");

		if (i == 0)
			pTile->setPosition(0, pTile->getContentSize().height + 720.0f);
		else if (i == 1)
		{
			pTile->setName("Tile");
			pTile->setPosition(67.5f, 226.0f + 720.0f);
		}
		else if(i == 2)
			pTile->setPosition(157.5f, 135.5f + 720.0f);
		else if (i == 3)
			pTile->setPosition(248.5f, 225.0f + 720.0f);
		else if (i == 4)
		{
			pTile->setName("Tile");
			pTile->setPosition(67.5f, 403.5f + 720.0f);
		}
		else if (i == 5)
		{
			pTile->setName("Tile");
			pTile->setPosition(473.2f, 315.0f + 720.0f);
		}
		else if (i == 6)
			pTile->setPosition(157.5f, 313.5f + 720.0f);
		else if (i == 7)
			pTile->setPosition(248.5f, 402.5f + 720.0f);
		else if (i == 8)
		{
			pTile->setName("Tile");
			pTile->setPosition(473.0f, 492.5f + 720.0f);
		}
		else if (i == 9)
			pTile->setPosition(113.5f, 495.5f + 720.0f);

		m_vcStageTile_01.pushBack(pTile);
		this->addChild(pTile);
	}
	m_pStageBg_02 = Sprite::createWithSpriteFrameName("stage_02_bg.png");
	m_pStageBg_02->setAnchorPoint(Vec2(0, 0));
	m_pStageBg_02->setPosition(Vec2(0, 720.0f));
	this->addChild(m_pStageBg_02);

	for (int i = 0; i < 6; i++)
	{
		sprintf(str, "stage_02_%02d.png", i);
		Sprite* pTile = Sprite::createWithSpriteFrameName(str);
		pTile->setAnchorPoint(Vec2(0, 1));
		pTile->setTag(i);
		pTile->setName("Normal");

		if (i == 0)
			pTile->setPosition(0, pTile->getContentSize().height + 1440.0f);
		else if (i == 1)
			pTile->setPosition(114.0f, 135.0f + 1440.0f);
		else if (i == 2)
			pTile->setPosition(0, 225.5f + 1440.0f);
		else if (i == 3)
			pTile->setPosition(406.5f, 270.0f + 1440.0f);
		else if (i == 4)
			pTile->setPosition(113.5f, 315.0f + 1440.0f);
		else if (i == 5)
			pTile->setPosition(0, 404.5f + 1440.0f);

		m_vcStageTile_02.pushBack(pTile);
		this->addChild(pTile);
	}

	m_pStageBg = m_pStageBg_00;
	for (auto i = m_vcStageTile_00.begin(); i != m_vcStageTile_00.end(); i++)
	{
		m_vcStageTile.pushBack((*i));
	}

	for (int i = 0; i < 4; i++)
	{
		Enemy* pEnemy = Enemy::create();
		pEnemy->Init(this, m_pIceCache, m_pMobRedCache, 0);

		if (i == 0)
			pEnemy->InitEnemy(Vec2(250, 550), EventKeyboard::KeyCode::KEY_RIGHT_ARROW);
		else if(i == 1)
			pEnemy->InitEnemy(Vec2(350, 550), EventKeyboard::KeyCode::KEY_LEFT_ARROW);
		else if(i == 2)
			pEnemy->InitEnemy(Vec2(140, 300), EventKeyboard::KeyCode::KEY_RIGHT_ARROW);
		else if(i == 3)
			pEnemy->InitEnemy(Vec2(450, 300), EventKeyboard::KeyCode::KEY_LEFT_ARROW);

		m_vcEnemy_00.pushBack(pEnemy);
	}
		
	for (int i = 0; i < 4; i++)
	{
		Enemy* pEnemy = Enemy::create();
		pEnemy->Init(this, m_pIceCache, m_pMobBeastCache, 1);

		if (i == 0)
			pEnemy->InitEnemy(Vec2(220, 550), EventKeyboard::KeyCode::KEY_RIGHT_ARROW);
		else if (i == 1)
			pEnemy->InitEnemy(Vec2(300, 450), EventKeyboard::KeyCode::KEY_LEFT_ARROW);
		else if (i == 2)
			pEnemy->InitEnemy(Vec2(220, 350), EventKeyboard::KeyCode::KEY_RIGHT_ARROW);
		else if (i == 3)
			pEnemy->InitEnemy(Vec2(300, 200), EventKeyboard::KeyCode::KEY_LEFT_ARROW);

		m_vcEnemy_01.pushBack(pEnemy);
		pEnemy->HideEnemy();
	}

	m_pBoss = Boss::create();
	m_pBoss->retain();
	m_pBoss->Init(m_pBossCache_00, this);
	for (auto i = m_vcEnemy_00.begin(); i != m_vcEnemy_00.end(); i++)
	{
		m_vcEnemy.pushBack((*i));
	}
}

void GameScene::PressKeyboard(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	if(m_bEnd)
		Director::getInstance()->replaceScene(TitleScene::create());
	if (m_bStart == false)
		return;
	if (m_pPlayer->GetLife() == false)
		return;
	if (m_bGameOver)
		return;

	switch (keyCode)
	{
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		{
			m_bKeyPush = true;
			m_pPlayer->Input(keyCode);
			m_iKeyCode = keyCode;
			break;
		}
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		{
			m_bKeyPush = true;
			m_pPlayer->Input(keyCode);
			m_iKeyCode = keyCode;
			break;
		}
		case EventKeyboard::KeyCode::KEY_SPACE:
		{
			m_pPlayer->Input(keyCode);
			m_iKeyCode = keyCode;
			break;
		}
		case EventKeyboard::KeyCode::KEY_A:
		{
			m_pPlayer->Input(keyCode);
			m_iKeyCode = keyCode;
			break;
		}
		case EventKeyboard::KeyCode::KEY_S:
		{
			if (m_pPlayer->GetSprite()->getPosition().y < 150)
				break;

			for (auto i = m_vcStageTile.begin(); i != m_vcStageTile.end(); i++)
				if ((*i)->getBoundingBox().intersectsRect(m_pPlayer->GetSprite()->getBoundingBox()))
					if ((*i)->getBoundingBox().getMidY() <= m_pPlayer->GetSprite()->getBoundingBox().getMinY())
						m_pPlayer->GetSprite()->setPosition(Vec2(m_pPlayer->GetSprite()->getPosition().x, m_pPlayer->GetSprite()->getPosition().y - 30));
			break;
		}
	}
}

void GameScene::CheckClearRound() // 라운드 클리어확인
{
	if (m_bEndRound)
		return;

	if (m_eGameStage != GAME_STAGE_THR)
	{
		int iMaxMonster = m_vcEnemy.size();
		int iCount = 0;
		for (auto i = m_vcEnemy.begin(); i != m_vcEnemy.end(); i++)
		{
			if ((*i)->GetDie())
				iCount++;
		}

		if (iCount == iMaxMonster)
		{
			m_bEndRound = true;
			EndRound();
		}
	}
}
void GameScene::EndRound() // 라운드 종료
{
	for (int i = 0; i < 5; i++)
	{
		Sprite* pItem = Sprite::createWithSpriteFrameName("item_00.png");

		if (i == 0)
			pItem->setPosition(Vec2(100, 800));
		else if (i == 1)
			pItem->setPosition(Vec2(250, 800));
		else if (i == 2)
			pItem->setPosition(Vec2(450, 800));
		else if (i == 3)
			pItem->setPosition(Vec2(500, 800));

		this->addChild(pItem, 2);
		m_vcItem.pushBack(pItem);
	}
	this->schedule(schedule_selector(GameScene::ChangeRound, this), 7.0f);
}
void GameScene::ChangeRound(float DelayTime) // 다음 라운드 
{
	this->unschedule(schedule_selector(GameScene::ChangeRound, this));

	if (m_eGameStage == GAME_STAGE_ONE)
		m_eGameStage = GAME_STAGE_TWO;
	else if (m_eGameStage == GAME_STAGE_TWO)
		m_eGameStage = GAME_STAGE_THR;

	m_bStart = false;

	m_pPlayer->ChangeTexture();

	auto pMoveByPlayer = MoveBy::create(1.8f, Vec2(0, 720));
	m_pPlayer->GetSprite()->runAction(pMoveByPlayer);

	for (auto i = m_vcEnemy.begin(); i != m_vcEnemy.end(); i++)
	{
		(*i)->DestroyEnemy();
	}

	if (m_eGameStage == GAME_STAGE_TWO)
	{
		auto pMoveBy00 = MoveBy::create(3.5f, Vec2(0, -720));
		m_pStageBg_00->runAction(pMoveBy00);
		auto pMoveBy01 = MoveBy::create(3.5f, Vec2(0, -720));
		auto pStart = CallFuncN::create(CC_CALLBACK_0(GameScene::StartNextRound, this));
		auto pSequence = Sequence::create(pMoveBy01, pStart, nullptr);
		m_pStageBg_01->runAction(pSequence);

		m_vcStageTile.clear();
		m_vcEnemy.clear();
		m_pStageBg = m_pStageBg_01;
		for (auto i = m_vcStageTile_01.begin(); i != m_vcStageTile_01.end(); i++)
		{
			m_vcStageTile.pushBack((*i));
		}
	}
	else if(m_eGameStage == GAME_STAGE_THR)
	{
		auto pMoveBy00 = MoveBy::create(3.5f, Vec2(0, -720));
		m_pStageBg_01->runAction(pMoveBy00);
		auto pMoveBy01 = MoveBy::create(3.5f, Vec2(0, -720));
		auto pStart = CallFuncN::create(CC_CALLBACK_0(GameScene::StartNextRound, this));
		auto pSequence = Sequence::create(pMoveBy01, pStart, nullptr);
		m_pStageBg_02->runAction(pSequence);

		m_vcStageTile.clear();
		m_vcEnemy.clear();
		m_pStageBg = m_pStageBg_02;
		for (auto i = m_vcStageTile_02.begin(); i != m_vcStageTile_02.end(); i++)
		{
			m_vcStageTile.pushBack((*i));
		}
	}

	for (auto i = m_vcStageTile_00.begin(); i != m_vcStageTile_00.end(); i++)
	{
		auto pMoveBy = MoveBy::create(3.5f, Vec2(0, -720));
		(*i)->runAction(pMoveBy);
	}
	for (auto i = m_vcStageTile_01.begin(); i != m_vcStageTile_01.end(); i++)
	{
		auto pMoveBy = MoveBy::create(3.5f, Vec2(0, -720));
		(*i)->runAction(pMoveBy);
	}
	for (auto i = m_vcStageTile_02.begin(); i != m_vcStageTile_02.end(); i++)
	{
		auto pMoveBy = MoveBy::create(3.5f, Vec2(0, -720));
		(*i)->runAction(pMoveBy);
	}
	for (auto i = m_vcItem.begin(); i != m_vcItem.end(); i++)
	{
		auto pMoveBy = MoveBy::create(3.5f, Vec2(0, -720));
		(*i)->runAction(pMoveBy);
	}

	SimpleAudioEngine::getInstance()->playEffect("sound/fx_stage_next_01.wav");
}


void GameScene::StartNextRound() // 다음 라운드 시작
{
	m_bStart = true;
	m_bEndRound = false;
	if (m_eGameStage == GAME_STAGE_TWO)
	{
		for (auto i = m_vcEnemy_01.begin(); i != m_vcEnemy_01.end(); i++)
		{
			(*i)->ShowEnemy();
			m_vcEnemy.pushBack((*i));
		}
	}
	else if (m_eGameStage == GAME_STAGE_THR)
	{
		this->schedule(schedule_selector(GameScene::GenerateEnemyBoss, this), 1.5f);
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/bgm_b_boss_01.mp3", true);
		SimpleAudioEngine::getInstance()->playEffect("sound/fx_stage_boss_01.wav");
	}
	m_vcItem.clear();
	m_pPlayer->GetSprite()->setVisible(false);
	m_pPlayer->RespawnNextRound();
}

void GameScene::ReleaseKeyboard(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	if (m_bStart == false)
		return;
	if (m_pPlayer->GetLife() == false)
		return;
	if (m_bGameOver)
		return;

	m_bKeyPush = false;
	m_pPlayer->ReleaseInput();
	m_iCombo = 0;
	for (auto i = m_vcEnemy.begin(); i != m_vcEnemy.end(); i++)
	{
		if ((*i)->GetDie())
			continue;
		(*i)->ReleasePush();
	}
}

void GameScene::MoveEnemy()
{
	for (auto i = m_vcEnemy.begin(); i != m_vcEnemy.end(); i++)
	{
		if ((*i)->GetDie())
			continue;
		(*i)->GetEnemy()->setTag(1);
	}
	for (auto i = m_vcStageTile.begin(); i != m_vcStageTile.end(); i++)
	{
		if ((*i)->getName() != "Tile")
			continue;
		for (auto j = m_vcEnemy.begin(); j != m_vcEnemy.end(); j++)
		{
			if ((*j)->GetDie())
				continue;

			if ((*i)->getBoundingBox().getMaxY() <= (*j)->GetEnemy()->getBoundingBox().getMidY())
				continue;
			if ((*i)->getBoundingBox().getMinY() >= (*j)->GetEnemy()->getBoundingBox().getMinY())
				continue;

			if ((*j)->GetWay() == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
			{
				if((*i)->getBoundingBox().getMinX() <= (*j)->GetEnemy()->getBoundingBox().getMaxX() + 1.0f && (*i)->getBoundingBox().getMaxX() >= (*j)->GetEnemy()->getBoundingBox().getMinX() && (*i)->getBoundingBox().getMinY() <= (*j)->GetEnemy()->getBoundingBox().getMaxY() && (*i)->getBoundingBox().getMaxY() >= (*j)->GetEnemy()->getBoundingBox().getMinY())
					(*j)->GetEnemy()->setTag(0);
			}
			else if ((*j)->GetWay() == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
			{
				if ((*i)->getBoundingBox().getMinX() <= (*j)->GetEnemy()->getBoundingBox().getMaxX() && (*i)->getBoundingBox().getMaxX() >= (*j)->GetEnemy()->getBoundingBox().getMinX() - 1.0f && (*i)->getBoundingBox().getMinY() <= (*j)->GetEnemy()->getBoundingBox().getMaxY() && (*i)->getBoundingBox().getMaxY() >= (*j)->GetEnemy()->getBoundingBox().getMinY())
					(*j)->GetEnemy()->setTag(0);
			}
		}
	}
	for (auto i = m_vcEnemy.begin(); i != m_vcEnemy.end(); i++)
	{
		if ((*i)->GetDie())
			continue;
		if((*i)->GetEnemy()->getTag() == 1)
			(*i)->MoveEnemy();
		else
			if ((*i)->GetState() == ENEMY_STATE_ICE_RUN)
			{
				(*i)->ChangeWay();
				(*i)->MoveEnemy();
			}
	}
}

void GameScene::SetGravity() // 플레이어 중력
{
	if (m_pPlayer->GetJump())
		return;
	if (m_pPlayer->GetLife() == false)
		return;

	for (auto i = m_vcStageTile.begin(); i != m_vcStageTile.end(); i++)
	{
		if ((*i)->getBoundingBox().intersectsRect(m_pPlayer->GetSprite()->getBoundingBox()))
		{
			if ((*i)->getBoundingBox().getMidY() <= m_pPlayer->GetSprite()->getBoundingBox().getMinY())
			{
				m_pPlayer->GetSprite()->setTag((*i)->getTag());
				m_pPlayer->ResetFall();
				return;
			}
		}
	}
	m_pPlayer->GetSprite()->setPosition(Vec2(m_pPlayer->GetSprite()->getPosition().x, m_pPlayer->GetSprite()->getPosition().y - 5));
	m_pPlayer->SetFall();
	m_pPlayer->GetSprite()->setTag(30);
}
void GameScene::SetGravityEnemy() // 적 중력
{
	for (auto i = m_vcEnemy.begin(); i != m_vcEnemy.end(); i++)
	{
		if ((*i)->GetDie())
			continue;

		(*i)->GetEnemy()->setTag(0);
	}
	for (auto i = m_vcStageTile.begin(); i != m_vcStageTile.end(); i++)
		for (auto j = m_vcEnemy.begin(); j != m_vcEnemy.end(); j++)
		{
			if ((*j)->GetDie())
				continue;
			if ((*i)->getBoundingBox().intersectsRect((*j)->GetEnemy()->getBoundingBox()))
				if ((*i)->getBoundingBox().getMaxY() - 10 <= (*j)->GetEnemy()->getBoundingBox().getMinY())
					(*j)->GetEnemy()->setTag(1);
		}

	for (auto i = m_vcEnemy.begin(); i != m_vcEnemy.end(); i++)
	{
		if ((*i)->GetDie())
			continue;
		if ((*i)->GetEnemy()->getTag() == 0)
		{
			if ((*i)->GetState() == ENEMY_STATE_ICE_RUN)
			{
				(*i)->GetEnemy()->setPosition(Vec2((*i)->GetEnemy()->getPosition().x, (*i)->GetEnemy()->getPosition().y - 15));
				(*i)->GetIce()->setPosition(Vec2((*i)->GetIce()->getPosition().x, (*i)->GetIce()->getPosition().y - 15));
			}
			else
			{
				(*i)->GetEnemy()->setPosition(Vec2((*i)->GetEnemy()->getPosition().x, (*i)->GetEnemy()->getPosition().y - 5));
				(*i)->GetIce()->setPosition(Vec2((*i)->GetIce()->getPosition().x, (*i)->GetIce()->getPosition().y - 5));
			}
		}
	}
}
void GameScene::SetGravityBoss() // 보스 중력
{
	if (m_eGameStage != GAME_STAGE_THR)
		return;
	if (m_pBoss->GetPattern() == BOSS_PATTERN_JUMP)
		return;

	if (m_pBoss->GetBoss()->getBoundingBox().getMinY() <= 48)
	{
		m_pBoss->CheckTile();
		return;
	}

	for (auto i = m_vcStageTile.begin(); i != m_vcStageTile.end(); i++)
	{
		if ((*i)->getBoundingBox().intersectsRect(m_pBoss->GetBoss()->getBoundingBox()))
		{
			m_pBoss->SpawnBoss();
			if ((*i)->getBoundingBox().getMaxY() - 6 >= m_pBoss->GetBoss()->getBoundingBox().getMinY())
				break;
			return;
		}
	}
	m_pBoss->GetBoss()->setPosition(Vec2(m_pBoss->GetBoss()->getPosition().x, m_pBoss->GetBoss()->getPosition().y - 5));
}

void GameScene::SetGravityItem() // 아이템 중력
{
	for (auto i = m_vcItem.begin(); i != m_vcItem.end(); i++)
	{
		if ((*i)->isVisible() == false)
			continue;
		if ((*i)->getPosition().y < 80)
			continue;

		(*i)->setPosition(Vec2((*i)->getPosition().x, (*i)->getPosition().y - 5));
	}
}

void GameScene::CheckCrashPlayerToEnemy() // 플레이어 적 충돌체크
{
	if (m_pPlayer->GetLife() == false)
		return;
	bool bPush = false;

	for (auto i = m_vcEnemy.begin(); i != m_vcEnemy.end(); i++)
	{
		if ((*i)->GetDie())
			continue;
		if ((*i)->GetEnemy()->getBoundingBox().intersectsRect(m_pPlayer->GetSprite()->getBoundingBox()))
		{
			if ((*i)->GetState() == ENEMY_STATE_ICE)
			{
				bPush = true;
				(*i)->MoveIce(m_pPlayer->GetWay(), m_pPlayer->GetSprite());
			}
		}
	}

	if (bPush)
	{
		m_pPlayer->SetPushIce();
	}
	else
	{
		m_pPlayer->ResetPushIce();
	}
}

bool GameScene::CheckCrashTile() // 플레이어 벽 충돌체크
{
	if (m_pPlayer->GetJump())
		return true;
	if (m_pPlayer->GetLife() == false)
		return true;

	for (auto i = m_vcStageTile.begin(); i != m_vcStageTile.end(); i++)
	{
		if ((*i)->getName() != "Tile")
			continue;

		if ((*i)->getBoundingBox().intersectsRect(m_pPlayer->GetSprite()->getBoundingBox()))
		{
			if ((*i)->getBoundingBox().getMinY() >= m_pPlayer->GetSprite()->getBoundingBox().getMinY())
				return false;
			if ((*i)->getBoundingBox().getMaxY() - 5 <= m_pPlayer->GetSprite()->getBoundingBox().getMinY())
				return false;

			if (m_iKeyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
			{
				if (m_pPlayer->GetWay() == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
					return false;
				if ((*i)->getBoundingBox().getMinX() <= m_pPlayer->GetSprite()->getBoundingBox().getMaxX())
					return true;
				else
					return false;
			}
			else if (m_iKeyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
			{
				if (m_pPlayer->GetWay() == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
					return false;

				if ((*i)->getBoundingBox().getMaxX() >= m_pPlayer->GetSprite()->getBoundingBox().getMinX())
					return true;	
				else
					return false;
			}
		}
	}
	return false;
}

void GameScene::CheckCrashBulletToEnemy() // 탄환 적 충돌체크
{
	if (m_pPlayer->GetAttack() == false)
		return;

	for (auto i = m_vcEnemy.begin(); i != m_vcEnemy.end(); i++)
	{
		if ((*i)->GetDie())
			continue;
		if ((*i)->GetEnemy()->getBoundingBox().intersectsRect(m_pPlayer->GetBullet()->getBoundingBox()))
		{
			(*i)->CountHit(m_pPlayer->GetWay());
			m_pPlayer->ResetAttack();
			m_iScore += 10;
		}
	}
	if (m_eGameStage == GAME_STAGE_THR)
	{
		if (m_pBoss->GetBoss()->getBoundingBox().intersectsRect(m_pPlayer->GetBullet()->getBoundingBox()))
		{
			m_pBoss->SetDamage(1);
			m_pPlayer->ResetAttack();
			m_iScore += 50;
		}
	}
}
void GameScene::CheckCrashPlayerToItem() // 플레이어 아이템 충돌체크
{
	for (auto i = m_vcItem.begin(); i != m_vcItem.end(); i++)
	{
		if ((*i)->isVisible() == false)
			continue;
		if ((*i)->getBoundingBox().intersectsRect(m_pPlayer->GetSprite()->getBoundingBox()))
		{
			(*i)->setVisible(false);
			Sprite* pNum = Sprite::createWithSpriteFrameName("ui_game_point_money_10000.png");
			pNum->setPosition(Vec2((*i)->getPosition().x, (*i)->getPosition().y));
			auto pHide = Hide::create();
			auto pDelay = DelayTime::create(2.0f);
			auto pSequence = Sequence::create(pDelay, pHide, nullptr);
			pNum->runAction(pSequence);
			this->addChild(pNum, 2);

			m_iScore += 10000;
			SimpleAudioEngine::getInstance()->playEffect("sound/fx_item_00_01.wav");
		}
	}
}
void GameScene::CheckCrashIceEnemyToEnemy() // 적 적 충돌체크
{
	for (auto i = m_vcEnemy.begin(); i != m_vcEnemy.end(); i++)
	{
		if ((*i)->GetDie())
			continue;
		for (auto j = m_vcEnemy.begin(); j != m_vcEnemy.end(); j++)
		{
			if ((*j)->GetDie())
				continue;
			if (i == j)
				continue;
			if ((*i)->GetIce()->getBoundingBox().intersectsRect((*j)->GetEnemy()->getBoundingBox()))
			{
				if ((*i)->GetState() == ENEMY_STATE_ICE_RUN)
				{
					if ((*j)->GetState() == ENEMY_STATE_ICE)
					{
						(*j)->SetIceRush((*i)->GetWay());						
					}
					if ((*j)->GetState() == ENEMY_STATE_ICE_RUN)
					{

					}
					else
					{
						if (m_iCombo != 4)
							m_iCombo++;
						if ((*j)->GetState() == ENEMY_STATE_STACK_ICE)
							(*j)->DestroyIce();

						m_iScore += 1000 * m_iCombo;

						char str[100] = { 0 };
						if (m_iCombo == 1)
							sprintf(str, "ui_game_point_kill_1000.png");
						else if (m_iCombo == 2)
							sprintf(str, "ui_game_point_kill_2000.png");
						else if (m_iCombo == 3)
							sprintf(str, "ui_game_point_kill_4000.png");
						else if (m_iCombo == 4)
							sprintf(str, "ui_game_point_kill_8000.png");

						Sprite* pNum = Sprite::createWithSpriteFrameName(str);
						pNum->setPosition(Vec2((*i)->GetEnemy()->getPosition().x, (*i)->GetEnemy()->getPosition().y));
						auto pHide = Hide::create();
						auto pDelay = DelayTime::create(2.0f);
						auto pSequence = Sequence::create(pDelay, pHide, nullptr);
						pNum->runAction(pSequence);
						this->addChild(pNum, 2);

						(*j)->DamageIce();
						SimpleAudioEngine::getInstance()->playEffect("sound/fx_enemy_kill_01.wav");
					}
				}
				if ((*i)->GetState() == ENEMY_STATE_STACK_ICE)
				{
					(*j)->ChangeWay();
				}
			}
		}
	}

	if (m_pPlayer->GetLife() == false)
		return;
	if (m_pPlayer->GetGod())
		return;

	bool bIceRush = false;
	for (auto i = m_vcEnemy.begin(); i != m_vcEnemy.end(); i++)
	{
		if ((*i)->GetDie())
			continue;

		if ((*i)->GetIce()->getBoundingBox().intersectsRect(m_pPlayer->GetSprite()->getBoundingBox()))
		{
			if ((*i)->GetState() == ENEMY_STATE_ICE_RUN)
			{
				m_pPlayer->GetSprite()->setPosition((*i)->GetIce()->getPosition().x + 40, (*i)->GetIce()->getPosition().y + 40);
				bIceRush = true;
			}
			else if ((*i)->GetState() == ENEMY_STATE_RUN)
			{
				m_pPlayer->SetDead();
				this->schedule(schedule_selector(GameScene::ReSpawnPlayer, this), 5.0f);
				return;
			}
		}
	}

	if(bIceRush)
		m_pPlayer->SetIceRunAnimation();
	else
		m_pPlayer->ResetIceRun();
}
void GameScene::CheckCrashBossToPlayer() // 보스 플레이어 충돌체크
{
	if (m_eGameStage != GAME_STAGE_THR)
		return;
	if (m_pBoss->GetDie())
		return;
	if (m_pPlayer->GetLife() == false)
		return;

	if (m_pBoss->GetBoss()->getBoundingBox().intersectsRect(m_pPlayer->GetSprite()->getBoundingBox()))
	{
		m_pPlayer->SetDead();
		this->schedule(schedule_selector(GameScene::ReSpawnPlayer, this), 5.0f);
		return;
	}
}

void GameScene::ReSpawnPlayer(float DelayTime) // 리스폰
{
	this->unschedule(schedule_selector(GameScene::ReSpawnPlayer, this));

	if (m_iLife == 0)
	{
		ShowGameOver();
		return;
	}
	m_iLife--;

	m_bKeyPush = false;

	this->schedule(schedule_selector(GameScene::SetGodMode, this), 3.0f);
	m_pPlayer->GetSprite()->setPosition(Vec2(320, 85));
	m_pPlayer->GetSprite()->stopAllActions();
	StartGame();

	char str[10] = { 0 };
	sprintf(str, "%d", m_iLife);
	m_pUi_LifeLabe->setString(str);
}
void GameScene::SetGodMode(float DelayTime) // 일시 무적
{
	this->unschedule(schedule_selector(GameScene::SetGodMode, this));
	m_pPlayer->SetGodMode();
}

void GameScene::CheckCrashItem() // 아이템 체크
{
	for (auto i = m_vcEnemy.begin(); i != m_vcEnemy.end(); i++)
	{
		if ((*i)->GetisItem() == false)
			continue;
		if ((*i)->GetItem()->isVisible() == false)
			continue;

		if ((*i)->GetItem()->getBoundingBox().intersectsRect(m_pPlayer->GetSprite()->getBoundingBox()))
		{
			if ((*i)->GetItem()->getName() == "item_01.png")
			{
				m_iScore += 400;

				Sprite* pNum = Sprite::createWithSpriteFrameName("ui_game_point_item_400.png");
				pNum->setPosition(Vec2((*i)->GetEnemy()->getPosition().x, (*i)->GetEnemy()->getPosition().y));
				auto pHide = Hide::create();
				auto pDelay = DelayTime::create(2.0f);
				auto pSequence = Sequence::create(pDelay, pHide, nullptr);
				pNum->runAction(pSequence);
				this->addChild(pNum, 2);
			}
			else if ((*i)->GetItem()->getName() == "item_02.png")
			{
				m_iScore += 800;

				Sprite* pNum = Sprite::createWithSpriteFrameName("ui_game_point_item_800.png");
				pNum->setPosition(Vec2((*i)->GetEnemy()->getPosition().x, (*i)->GetEnemy()->getPosition().y));
				auto pHide = Hide::create();
				auto pDelay = DelayTime::create(2.0f);
				auto pSequence = Sequence::create(pDelay, pHide, nullptr);
				pNum->runAction(pSequence);
				this->addChild(pNum, 2);
			}
			else if ((*i)->GetItem()->getName() == "item_03.png")
			{
				m_pPlayer->SetSpeed();
				m_pPlayer->SetAnimation(m_pPlayer->GetWay());
			}
			(*i)->GetItem()->setVisible(false);
			SimpleAudioEngine::getInstance()->playEffect("sound/fx_item_00_01.wav");
		}
	}
}

void GameScene::ShowGameOver() // 게임오버
{
	m_bGameOver = true;
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	SimpleAudioEngine::getInstance()->playEffect("sound/fx_gameover_00_01.wav", false);
	for (auto i = m_vcStageTile.begin(); i != m_vcStageTile.end(); i++)
	{
		(*i)->setVisible(false);
	}
	for (auto i = m_vcEnemy.begin(); i != m_vcEnemy.end(); i++)
	{
		(*i)->DestroyEnemy();
	}
	m_pStageBg->setVisible(false);
	m_pPlayer->GetSprite()->setVisible(false);
	m_pUi_Player->setVisible(false);
	m_pUi_LifeIcon->setVisible(false);
	m_pUi_ScoreLabel->setVisible(false);
	m_pUi_LifeLabe->setVisible(false);
	m_pBoss->GetBoss()->setVisible(false);

	Sprite* pGameOverAnimation = Sprite::createWithSpriteFrameName("ui_gameover01_00.png");
	pGameOverAnimation->setPosition(ResManager::GetInstance()->GetDisplaySize().width / 2, ResManager::GetInstance()->GetDisplaySize().height / 2 + 100);
	Sprite* pGameOverEnemy = Sprite::createWithSpriteFrameName("ui_gameover02_00.png");
	pGameOverEnemy->setPosition(ResManager::GetInstance()->GetDisplaySize().width / 2, ResManager::GetInstance()->GetDisplaySize().height / 2);

	Vector<SpriteFrame*> pAniFramesOne;
	Vector<SpriteFrame*> pAniFramesTwo;
	char str[100] = { 0 };

	for (int i = 0; i < 10; i++)
	{
		sprintf(str, "ui_gameover01_%02d.png", i);
		SpriteFrame* Frame = m_pUICache->getSpriteFrameByName(str);
		pAniFramesOne.pushBack(Frame);
	}
	for (int i = 10; i < 12; i++)
	{
		sprintf(str, "ui_gameover01_%d.png", i);
		SpriteFrame* Frame = m_pUICache->getSpriteFrameByName(str);
		pAniFramesOne.pushBack(Frame);
	}
	for (int i = 0; i < 2; i++)
	{
		sprintf(str, "ui_gameover02_%02d.png", i);
		SpriteFrame* Frame = m_pUICache->getSpriteFrameByName(str);
		pAniFramesTwo.pushBack(Frame);
	}

	Animation* pAnimationOne = Animation::createWithSpriteFrames(pAniFramesOne, 0.15f);
	Animate* pAnimateOne = Animate::create(pAnimationOne);
	pGameOverAnimation->runAction(pAnimateOne);

	Animation* pAnimationTwo = Animation::createWithSpriteFrames(pAniFramesTwo, 0.1f);
	Animate* pAnimateTwo = Animate::create(pAnimationTwo);
	auto pRepeat = RepeatForever::create(pAnimateTwo);
	pGameOverEnemy->runAction(pRepeat);

	this->addChild(pGameOverAnimation);
	this->addChild(pGameOverEnemy);
	m_bEnd = true;
}

void GameScene::GenerateEnemyBoss(float DelayTime) // 보스 스테이지 몬스터 소환
{
	if (m_eGameStage != GAME_STAGE_THR)
		return;
	if (m_pBoss->GetPattern() != BOSS_PATTERN_ATTACK)
		return;
	if (m_pBoss->GetEnemyIndex() == 0)
		return;

	m_pBoss->RemoveEnemyIndex();

	Enemy* pEnemy = Enemy::create();
	pEnemy->Init(this, m_pIceCache, m_pMobRedCache, 0);
	pEnemy->InitEnemy(Vec2(m_pBoss->GetBoss()->getBoundingBox().getMinX(), m_pBoss->GetBoss()->getBoundingBox().getMaxY() - 40.0f), EventKeyboard::KeyCode::KEY_LEFT_ARROW);
	auto pMoveBy = MoveBy::create(0.5f, Vec2(-280 - (m_pBoss->GetEnemyIndex() * 50), 200));
	pEnemy->GetEnemy()->runAction(pMoveBy);
	m_vcEnemy.pushBack(pEnemy);
}
void GameScene::CheckCrashEnemyToBoss() // 적 보스 충돌체크
{
	if (m_eGameStage != GAME_STAGE_THR)
		return;
	for (auto i = m_vcEnemy.begin(); i != m_vcEnemy.end(); i++)
	{
		if ((*i)->GetDie())
			continue;
		if ((*i)->GetState() != ENEMY_STATE_ICE_RUN)
			continue;
		if ((*i)->GetEnemy()->getBoundingBox().intersectsRect(m_pBoss->GetBoss()->getBoundingBox()))
		{
			m_pBoss->SetDamage(5);
			(*i)->EffectExplosion();
			if (m_pPlayer->GetIceRun())
			{
				m_pPlayer->ChangeGodMode();
				this->schedule(schedule_selector(GameScene::SetGodMode, this), 3.0f);
			}
			return;
		}
	}
}

void GameScene::CheckDeadBoss() // 보스 잡은거 체크
{
	if (m_eGameStage != GAME_STAGE_THR)
		return;

	if (m_pBoss->GetHp() <= 0)
	{
		this->unschedule(schedule_selector(GameScene::GenerateEnemyBoss, this));
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		for (auto i = m_vcEnemy.begin(); i != m_vcEnemy.end(); i++)
		{
			(*i)->DestroyEnemy();
		}
		m_bStart = false;
		m_pBoss->SetDead();
		Sprite* pClearText = Sprite::createWithSpriteFrameName("ui_game_con.png");
		pClearText->setPosition(Vec2(ResManager::GetInstance()->GetDisplaySize().width / 2, ResManager::GetInstance()->GetDisplaySize().height + 10));
		auto pMoveTo = MoveTo::create(2.0f, Vec2(ResManager::GetInstance()->GetDisplaySize().width / 2, ResManager::GetInstance()->GetDisplaySize().height / 2));
		auto pDelayTime = DelayTime::create(3.0f);
		auto pSequence = Sequence::create(pDelayTime, pMoveTo, nullptr);
		pClearText->runAction(pSequence);
		this->addChild(pClearText, 40);
		this->schedule(schedule_selector(GameScene::BackTitleScene, this), 10.0f);
	}
}
void GameScene::BackTitleScene(float DelayTime) // 타이틀 신 이동
{
	Director::getInstance()->replaceScene(TitleScene::create());
}
