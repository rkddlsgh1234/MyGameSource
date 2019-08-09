#include "ObjectManager.h"
#include "ResManager.h"
#include "GameManager.h"
#include <random>

#include <map>

#include <iostream>
#include <fstream>


USING_NS_CC;

ObjectManager::ObjectManager()
{
}


ObjectManager::~ObjectManager()
{
	m_vcGameBackGround.clear();
	m_vcStageRoad.clear();
	m_vcEnemy.clear();
	m_vcBackGroundStage00.clear();
	m_vcBackGroundStage01.clear();
	m_vcBackGroundStage02.clear();
}
void ObjectManager::Init(Scene* pScene)
{
	m_eGameStage = GAME_STAGE_ONE; // 스테이지 레벨
	m_bStartState = false; // 시작 상태
	m_bFever = false;
	m_iStageRoofCount = 0; // 배경 루프 횟수
	m_iBounsTime = 0;
	m_iStageRoofMax = 14;
	m_iSpeed = BACK_SPEED;
	m_pScene = pScene; // 게임신 레이블

	InitCache();
	InitLoading();
	InitPlayGame();
}
void ObjectManager::InitPlayer(cocos2d::Sprite* pPlayer)
{
	m_pPlayer = pPlayer;
}
void ObjectManager::InitLoading()
{
	m_pLoadingBackGround = Sprite::createWithSpriteFrameName("loading_bg.png");
	ResManager::GetInstance()->SetFullSizeSprite(m_pLoadingBackGround);
	m_pScene->addChild(m_pLoadingBackGround, 2);

	m_pDelay = Sprite::createWithSpriteFrameName("loading_bg_effect.png");
	m_pScene->addChild(m_pDelay);

	m_pLoadingBackEffect = Sprite::createWithSpriteFrameName("loading_bg_effect.png");
	m_pLoadingBackEffect->setScale(6);
	m_pLoadingBackEffect->setPosition(Vec2(ResManager::GetInstance()->GetDisplaySize().width / 2, ResManager::GetInstance()->GetDisplaySize().height / 2 - 30));
	m_pScene->addChild(m_pLoadingBackEffect, 0);

	char str[100] = { 0 };
	sprintf(str, "main_character_%02d.png", GameManager::GetInstance()->GetUserData()->iCharacter);
	m_pLoadingCharacter = Sprite::createWithSpriteFrameName(str);
	m_pLoadingCharacter->setScale(3);
	m_pLoadingCharacter->setPosition(Vec2(ResManager::GetInstance()->GetDisplaySize().width / 2 - 10, ResManager::GetInstance()->GetDisplaySize().height / 2 - 90));
	m_pScene->addChild(m_pLoadingCharacter);
}
void ObjectManager::InitCache()
{
	m_pCharacterCache = SpriteFrameCache::sharedSpriteFrameCache();
	m_pCharacterCache->addSpriteFramesWithFile("main_character.plist");
	m_pLoadingCache = SpriteFrameCache::sharedSpriteFrameCache();
	m_pLoadingCache->addSpriteFramesWithFile("loading.plist");
	m_pBonusCache = SpriteFrameCache::sharedSpriteFrameCache();
	m_pBonusCache->addSpriteFramesWithFile("bonus_bg.plist");
	
	m_pStageCache_00 = SpriteFrameCache::sharedSpriteFrameCache();
	m_pStageCache_00->addSpriteFramesWithFile("stage_tm_00.plist");
	m_pStageCache_01 = SpriteFrameCache::sharedSpriteFrameCache();
	m_pStageCache_01->addSpriteFramesWithFile("stage_tm_01.plist");
	m_pStageCache_01 = SpriteFrameCache::sharedSpriteFrameCache();
	m_pStageCache_01->addSpriteFramesWithFile("stage_tm_02.plist");
}
void ObjectManager::InitPlayGame()
{
	for (int i = 0; i < 2; i++)
	{
		Sprite* pBackGround_00 = Sprite::create("stage_bg_00.png");
		ResManager::GetInstance()->SetFullSizeSprite(pBackGround_00);
		pBackGround_00->setVisible(false);
		pBackGround_00->setTag(i);
		if (i == 0)
			m_fBackSize = pBackGround_00->getBoundingBox().getMaxX();
		else
			pBackGround_00->setPosition(Vec2(m_fBackSize, 0));

		m_vcBackGroundStage00.pushBack(pBackGround_00);
		m_pScene->addChild(pBackGround_00, 0);
	}
	for (int i = 0; i < 4; i++)
	{
		Sprite* pBackBonus;
		if (i == 2 || i == 3)
			pBackBonus = Sprite::createWithSpriteFrameName("bonus_bg_00.png");
		else
			pBackBonus = Sprite::createWithSpriteFrameName("bonus_bg_01.png");
		ResManager::GetInstance()->SetFullSizeSprite(pBackBonus);
		pBackBonus->setTag(i);
		pBackBonus->setVisible(false);
		pBackBonus->setOpacity(0);
		if (i == 1 || i == 3)
			pBackBonus->setPosition(Vec2(m_fBackSize, 0));
		m_vcBonusBackGround.pushBack(pBackBonus);
		m_pScene->addChild(pBackBonus, 0);
	}

	for (int i = 0; i < 4; i++)
	{
		Sprite* pBackGround_01; 
		if(i == 2 || i == 3)
			pBackGround_01 = Sprite::createWithSpriteFrameName("stage_bg_01.png");
		else
			pBackGround_01 = Sprite::createWithSpriteFrameName("stage_bg_01_effect.png");
		ResManager::GetInstance()->SetFullSizeSprite(pBackGround_01);
		pBackGround_01->setTag(i);
		pBackGround_01->setVisible(false);
		if (i == 1 || i == 3)
			pBackGround_01->setPosition(Vec2(m_fBackSize, 0));
		m_vcBackGroundStage01.pushBack(pBackGround_01);
		m_pScene->addChild(pBackGround_01, 0);
	}

	for (int i = 0; i < 4; i++)
	{
		Sprite* pBackGround_02;
		if (i == 0 || i == 1)
			pBackGround_02 = Sprite::createWithSpriteFrameName("stage_bg_02.png");
		else
			pBackGround_02 = Sprite::createWithSpriteFrameName("stage_bg_02_effect.png");
		ResManager::GetInstance()->SetFullSizeSprite(pBackGround_02);
		pBackGround_02->setTag(i);
		pBackGround_02->setVisible(false);
		if (i == 1 || i == 3)
			pBackGround_02->setPosition(Vec2(m_fBackSize, 0));
		m_vcBackGroundStage02.pushBack(pBackGround_02);
		m_pScene->addChild(pBackGround_02, 0);
	}

	// 스테이지 1
	Sprite* pObjectStage;
	for (int i = 0; i < STAGE_00_ROAD; i++)
	{
		Sprite* pObjectStage_00 = Sprite::createWithSpriteFrameName("stage_tm_load.png");
		pObjectStage_00->setAnchorPoint(Vec2(0, 0));
		pObjectStage_00->setScale(1.5);
		pObjectStage_00->setVisible(false);
		pObjectStage_00->setPosition(Vec2(i * (pObjectStage_00->getContentSize().width * 1.5), 0));
		m_vcStageRoad.pushBack(pObjectStage_00);
		m_pScene->addChild(pObjectStage_00, 1);

		if (i == STAGE_00_ROAD - 1)
			pObjectStage = pObjectStage_00;
	}
	// 스테이지 2
	for (int i = 0; i < STAGE_01_ROAD; i++)
	{
		if (i == 30 || i == 31 || i == 60 || i == 61 || i == 62 || i == 98 || i == 99 || i == 100 || i == 101)
			continue;
		Sprite* pObjectStage_01 = Sprite::createWithSpriteFrameName("stage_tm01_02.png");
		pObjectStage_01->setAnchorPoint(Vec2(0, 0));
		pObjectStage_01->setScale(1.5);
		pObjectStage_01->setVisible(false);
		pObjectStage_01->setPosition(Vec2(pObjectStage->getBoundingBox().getMaxX() + (i * (pObjectStage_01->getContentSize().width * 1.5)), 0));
		m_vcStageRoad.pushBack(pObjectStage_01);
		m_pScene->addChild(pObjectStage_01, 1);

		if (i == STAGE_01_ROAD - 1)
			pObjectStage = pObjectStage_01;
	}

	for (int i = 0; i < 125; i++)
	{
		if (i == 50 || i == 51 || i == 52)
			continue;
		Sprite* pObjectStage_01 = Sprite::createWithSpriteFrameName("stage_tm02_road.png");
		pObjectStage_01->setAnchorPoint(Vec2(0, 0));
		pObjectStage_01->setScale(1.5);
		pObjectStage_01->setVisible(false);
		pObjectStage_01->setPosition(Vec2(pObjectStage->getBoundingBox().getMaxX() + (i * (pObjectStage_01->getContentSize().width * 1.5)), 0));
		m_vcStageRoad.pushBack(pObjectStage_01);
		m_pScene->addChild(pObjectStage_01, 1);
	}


	SetObject(pObjectStage);
	SetItem(pObjectStage);
	SetPoint(pObjectStage);
}
void ObjectManager::InitStart()
{
	m_bStartState = true;

	m_pLoadingBackGround->setVisible(false);
	m_pLoadingBackEffect->setVisible(false);
	m_pLoadingCharacter->setVisible(false);
	
	for (auto i = m_vcBackGroundStage00.begin(); i != m_vcBackGroundStage00.end(); i++)
	{
		(*i)->setVisible(true);
		m_vcGameBackGround.pushBack((*i));
	}
	for (auto i = m_vcStageRoad.begin(); i != m_vcStageRoad.end(); i++)
	{
		(*i)->setVisible(true);
	}
	for (auto i = m_vcEnemy.begin(); i != m_vcEnemy.end(); i++)
	{
		(*i)->setVisible(true);
	}
	for (auto i = m_vcItem.begin(); i != m_vcItem.end(); i++)
	{
		(*i)->setVisible(true);
	}
	for (auto i = m_vcPoint.begin(); i != m_vcPoint.end(); i++)
	{
		(*i)->setVisible(true);
	}

	DelayTime* pDelay = DelayTime::create(6.0f);
	auto pGenerateBouns = CallFuncN::create(CC_CALLBACK_0(ObjectManager::GenerateBonusTime, this));
	Sequence* pSequence = Sequence::create(pDelay, pGenerateBouns, nullptr);
	RepeatForever* pRepeat = RepeatForever::create(pSequence);
	m_pDelay->runAction(pRepeat);
}

void ObjectManager::GenerateBonusTime()
{
	if (m_bFever)
		return;
	char str[100] = { 0 };
	sprintf(str, "play_bonus_%02d.png", m_iBounsTime);
	Sprite* pBouns = Sprite::createWithSpriteFrameName(str);
	pBouns->setPosition(Vec2(ResManager::GetInstance()->GetDisplaySize().width, 150));
	pBouns->setName(str);
	m_pScene->addChild(pBouns, 3);
	m_vcPoint.pushBack(pBouns);

	m_iBounsTime++;

	if (m_iBounsTime == 9)
		m_iBounsTime = 0;
}

void ObjectManager::SetObject(cocos2d::Sprite* pSprite)
{
	map < int, ENEMY_OBJECT> mapEnemy;

	ifstream MapData("map_data.txt");
	int iLen;
	MapData >> iLen;
	for (int i = 0; i < iLen; i++)
	{
		int iPos = 0;
		int iObject = 0;
		MapData >> iPos >> iObject;
		mapEnemy.insert(pair<int, ENEMY_OBJECT>(iPos, (ENEMY_OBJECT)iObject));
	}

	for (auto iter = mapEnemy.begin(); iter != mapEnemy.end(); iter++)
	{
		Sprite* pObjectEnemy;

		if (iter->second == STAGE_00_ENEMY_OBJECT_UP)
		{
			pObjectEnemy = Sprite::createWithSpriteFrameName("stage_tm_01.png");
			pObjectEnemy->setPosition(Vec2((*iter).first * (pSprite->getContentSize().width * 1.5f), ResManager::GetInstance()->GetDisplaySize().height - pObjectEnemy->getContentSize().height * 2.2));
			pObjectEnemy->setScaleX(2.0f);
			pObjectEnemy->setScaleY(2.2f);
		}
		else if (iter->second == STAGE_00_ENEMY_OBJECT_DOWN_SHORT)
		{
			pObjectEnemy = Sprite::createWithSpriteFrameName("stage_tm_00.png");
			pObjectEnemy->setPosition(Vec2((*iter).first * (pSprite->getContentSize().width * 1.5f), pSprite->getBoundingBox().getMaxY() - 5));
			pObjectEnemy->setScale(2.0f);
		}
		else if (iter->second == STAGE_00_ENEMY_OBJECT_DOWN_LONG)
		{
			pObjectEnemy = Sprite::createWithSpriteFrameName("stage_tm_02.png");
			pObjectEnemy->setPosition(Vec2((*iter).first * (pSprite->getContentSize().width * 1.5f), pSprite->getBoundingBox().getMaxY() - 5));
			pObjectEnemy->setScale(2.0f);
		}
		else if ((*iter).second == STAGE_01_ENEMY_OBJECT_UP)
		{
			pObjectEnemy = Sprite::createWithSpriteFrameName("stage_tm01_00.png");
			pObjectEnemy->setPosition(Vec2((*iter).first * (pSprite->getContentSize().width * 1.5f), ResManager::GetInstance()->GetDisplaySize().height - pObjectEnemy->getContentSize().height * 2.2));
			pObjectEnemy->setScaleX(2.0f);
			pObjectEnemy->setScaleY(2.2f);
		}
		else if ((*iter).second == STAGE_01_ENEMY_OBJECT_DOWN_SHORT)
		{
			pObjectEnemy = Sprite::createWithSpriteFrameName("stage_tm01_03.png");
			pObjectEnemy->setPosition(Vec2((*iter).first * (pSprite->getContentSize().width * 1.5f), pSprite->getBoundingBox().getMaxY() - 5));
			pObjectEnemy->setScale(2.0f);
		}
		else if ((*iter).second == STAGE_01_ENEMY_OBJECT_DOWN_LONG)
		{
			pObjectEnemy = Sprite::createWithSpriteFrameName("stage_tm01_01.png");
			pObjectEnemy->setPosition(Vec2((*iter).first * (pSprite->getContentSize().width * 1.5f), pSprite->getBoundingBox().getMaxY() - 5));
			pObjectEnemy->setScale(2.0);
		}
		else if ((*iter).second == STAGE_02_ENEMY_OBJECT_UP)
		{
			pObjectEnemy = Sprite::createWithSpriteFrameName("stage_tm02_01.png");
			pObjectEnemy->setPosition(Vec2((*iter).first * (pSprite->getContentSize().width * 1.5f), ResManager::GetInstance()->GetDisplaySize().height - pObjectEnemy->getContentSize().height * 2.2));
			pObjectEnemy->setScaleX(2.0f);
			pObjectEnemy->setScaleY(2.2f);
		}
		if (iter->second == STAGE_02_ENEMY_OBJECT_DOWN_SHORT)
		{
			pObjectEnemy = Sprite::createWithSpriteFrameName("stage_tm02_00.png");
			pObjectEnemy->setPosition(Vec2((*iter).first * (pSprite->getContentSize().width * 1.5f), pSprite->getBoundingBox().getMaxY() - 5));
			pObjectEnemy->setScale(2.0f);
		}
		else if (iter->second == STAGE_02_ENEMY_OBJECT_DOWN_LONG)
		{
			pObjectEnemy = Sprite::createWithSpriteFrameName("stage_tm02_02_00.png");
			pObjectEnemy->setPosition(Vec2((*iter).first * (pSprite->getContentSize().width * 1.5f), pSprite->getBoundingBox().getMaxY() - 5));
			pObjectEnemy->setScale(2.0f);
		}
		pObjectEnemy->setAnchorPoint(Vec2(0, 0));

		pObjectEnemy->setVisible(false);
		m_vcEnemy.pushBack(pObjectEnemy);
		m_pScene->addChild(pObjectEnemy, 1);
	}
}
void ObjectManager::SetItem(cocos2d::Sprite* pSprite)
{
	map < int, string> mapItem;

	mapItem.insert(pair<int, string>(200, "play_item_speed.png"));
	mapItem.insert(pair<int, string>(85, "play_item_big.png"));
	mapItem.insert(pair<int, string>(170, "play_item_small_health.png"));
	mapItem.insert(pair<int, string>(130, "play_item_big_health.png"));
	mapItem.insert(pair<int, string>(STAGE_00_ROAD + STAGE_01_ROAD - 10, "play_item_big_health.png"));

	for (auto iter = mapItem.begin(); iter != mapItem.end(); iter++)
	{
		Sprite* pObjectEnemy;
		pObjectEnemy = Sprite::createWithSpriteFrameName(iter->second);

		if(iter->second == "play_item_big_health.png")
			pObjectEnemy->setPosition(Vec2((*iter).first * (pSprite->getContentSize().width * 1.5f), pSprite->getBoundingBox().getMaxY() + 100));
		else
			pObjectEnemy->setPosition(Vec2((*iter).first * (pSprite->getContentSize().width * 1.5f), pSprite->getBoundingBox().getMaxY() + 50));
		MoveBy* pMoveBy = MoveBy::create(0.2f, Vec2(0, 10));
		Sequence* pSequence = Sequence::create(pMoveBy, pMoveBy->reverse(), nullptr);
		RepeatForever* pRepeatForever = RepeatForever::create(pSequence);
		pObjectEnemy->runAction(pRepeatForever);
		pObjectEnemy->setName(iter->second);
		pObjectEnemy->setVisible(false);
		if(iter->second == "play_item_big_health.png")
			pObjectEnemy->setScale(1.0f);
		else
			pObjectEnemy->setScale(0.7f);
		m_vcItem.pushBack(pObjectEnemy);
		m_pScene->addChild(pObjectEnemy, 1);
	}
}
void ObjectManager::SetPoint(cocos2d::Sprite* pSprite)
{
	map < int, int> mapPoint;
	int iPointX[30];
	int iPointY[30];

	mapPoint.insert(pair<int, int>(21, 21 + JELLY_POS_JUMP));
	mapPoint.insert(pair<int, int>(50, 50 + JELLY_POS_DUBBLE_JUMP));
	iPointX[0] = 21, iPointY[0] = iPointX[0] + JELLY_POS_JUMP;
	iPointX[1] = 43, iPointY[1] = iPointX[1] + JELLY_POS_JUMP;
	iPointX[2] = 210, iPointY[2] = iPointX[2] + JELLY_POS_DUBBLE_JUMP;
	iPointX[3] = 242, iPointY[3] = iPointX[3] + JELLY_POS_DUBBLE_JUMP;
	iPointX[4] = 266, iPointY[4] = iPointX[4] + JELLY_POS_DUBBLE_JUMP;
	iPointX[5] = 290, iPointY[5] = iPointX[5] + JELLY_POS_JUMP;
	iPointX[6] = 300, iPointY[6] = iPointX[6] + JELLY_POS_JUMP;
	iPointX[7] = 310, iPointY[7] = iPointX[7] + JELLY_POS_JUMP;
	iPointX[8] = 320, iPointY[8] = iPointX[8] + JELLY_POS_JUMP;
	iPointX[9] = 330, iPointY[9] = iPointX[9] + JELLY_POS_JUMP;
	iPointX[10] = 350, iPointY[10] = iPointX[10] + JELLY_POS_DUBBLE_JUMP;
	iPointX[11] = 365, iPointY[11] = iPointX[11] + JELLY_POS_DUBBLE_JUMP;
	iPointX[12] = 379, iPointY[12] = iPointX[12] + JELLY_POS_DUBBLE_JUMP;
	iPointX[13] = 393, iPointY[13] = iPointX[13] + JELLY_POS_DUBBLE_JUMP;
	iPointX[14] = 415, iPointY[14] = iPointX[14] + JELLY_POS_DUBBLE_JUMP;
	iPointX[15] = 430, iPointY[15] = iPointX[15] + 10;
	iPointX[16] = 440, iPointY[16] = iPointX[16] + JELLY_POS_JUMP;
	iPointX[17] = 446, iPointY[17] = iPointX[17] + JELLY_POS_JUMP;
	iPointX[18] = 447, iPointY[18] = iPointX[18] + JELLY_POS_JUMP;
	iPointX[19] = 460, iPointY[19] = iPointX[19] + JELLY_POS_DUBBLE_JUMP;
	iPointX[20] = 490, iPointY[20] = iPointX[20] + JELLY_POS_DUBBLE_JUMP;
	iPointX[21] = 520, iPointY[21] = iPointX[21] + JELLY_POS_JUMP;
	iPointX[22] = 540, iPointY[22] = iPointX[22] + JELLY_POS_JUMP;
	iPointX[23] = 560, iPointY[23] = iPointX[23] + JELLY_POS_JUMP;
	iPointX[24] = 620, iPointY[24] = iPointX[24] + JELLY_POS_JUMP;
	iPointX[25] = 625, iPointY[25] = iPointX[25] + JELLY_POS_DUBBLE_JUMP;
	iPointX[26] = 667, iPointY[26] = iPointX[26] + JELLY_POS_DUBBLE_JUMP;
	char str[100] = { 0 };
	sprintf(str, "shop_jelly_%02d.png", GameManager::GetInstance()->GetUserData()->iJellyLevel - 1);

	int iSize;
	if (str == "shop_jelly_00.png")
		iSize = 19;
	else
		iSize = 15;

	int iCount = 0;
	int iLen = 0;
	int iPointLen = 0;
	int iPointMax = 27;
	bool bUpPoint = false;
	for (int i = 0; i < 900; i++)
	{
		if (i >= 55 && i <= 200)
			continue;

		Sprite* pObjectEnemy;
		


		if (i >= iPointX[iPointLen] && i <= iPointY[iPointLen])
		{
			pObjectEnemy = Sprite::create("shop_show_coin.png");
			pObjectEnemy->setName("Coin");
			if (bUpPoint)
				iCount--;
			else
				iCount++;

			if ((iPointY[iPointLen] - iPointX[iPointLen]) / 2 == iCount)
				bUpPoint = true;

			pObjectEnemy->setPosition(Vec2(((i + 30) * iSize) + (iLen * 30), pSprite->getBoundingBox().getMaxY() + 40 + (iCount * 40)));
			if (i == iPointY[iPointLen])
			{
				bUpPoint = false;
				iCount = 0;

				if (iPointLen != iPointMax)
					iPointLen++;
			}
		}
		else
		{
			pObjectEnemy = Sprite::createWithSpriteFrameName(str);
			pObjectEnemy->setPosition(Vec2(((i + 30) * iSize) + (iLen * 30), pSprite->getBoundingBox().getMaxY() + 45));
			pObjectEnemy->setName("Jelly");
		}

		iLen++;
		pObjectEnemy->setScale(1.5f);
		pObjectEnemy->setVisible(false);
		m_vcPoint.pushBack(pObjectEnemy);
		m_pScene->addChild(pObjectEnemy, 1);
	}
}

bool ObjectManager::Updata()
{
	CheckNextChage();

	return false;
}
void ObjectManager::MoveObject()
{
	if (m_bFever)
	{
		for (auto i = m_vcBonusBackGround.begin(); i != m_vcBonusBackGround.end(); i++)
		{
			if ((*i)->getTag() == 2 || (*i)->getTag() == 3)
				(*i)->setPosition(Vec2((*i)->getPosition().x - m_iSpeed + 2, (*i)->getPosition().y));
			else
				(*i)->setPosition(Vec2((*i)->getPosition().x - m_iSpeed, (*i)->getPosition().y));

		}
		for (auto i = m_vcBonusItem.begin(); i != m_vcBonusItem.end(); i++)
		{
			if ((*i)->isVisible() == false)
				continue;
			(*i)->setPosition(Vec2((*i)->getPosition().x - m_iSpeed, (*i)->getPosition().y));
		}
	}
	else
	{
		for (auto i = m_vcGameBackGround.begin(); i != m_vcGameBackGround.end(); i++)
		{
			if ((*i)->getTag() == 2 || (*i)->getTag() == 3)
				(*i)->setPosition(Vec2((*i)->getPosition().x - m_iSpeed + 2, (*i)->getPosition().y));
			else
				(*i)->setPosition(Vec2((*i)->getPosition().x - m_iSpeed, (*i)->getPosition().y));

		}

		for (auto i = m_vcStageRoad.begin(); i != m_vcStageRoad.end(); i++)
		{
			if ((*i)->isVisible() == false)
				continue;
			(*i)->setPosition(Vec2((*i)->getPosition().x - m_iSpeed, (*i)->getPosition().y));
		}
		for (auto i = m_vcEnemy.begin(); i != m_vcEnemy.end(); i++)
		{
			if ((*i)->isVisible() == false)
				continue;
			(*i)->setPosition(Vec2((*i)->getPosition().x - m_iSpeed, (*i)->getPosition().y));
		}
		for (auto i = m_vcItem.begin(); i != m_vcItem.end(); i++)
		{
			if ((*i)->isVisible() == false)
				continue;
			(*i)->setPosition(Vec2((*i)->getPosition().x - m_iSpeed, (*i)->getPosition().y));
		}
		for (auto i = m_vcPoint.begin(); i != m_vcPoint.end(); i++)
		{
			if ((*i)->isVisible() == false)
				continue;
			(*i)->setPosition(Vec2((*i)->getPosition().x - m_iSpeed, (*i)->getPosition().y));
		}
	}
}
void ObjectManager::CheckNextChage()
{
	if (m_bFever)
	{
		for (auto i = m_vcBonusBackGround.begin(); i != m_vcBonusBackGround.end(); i++)
		{
			if ((*i)->getPosition().x <= -m_fBackSize)
			{
				(*i)->setPosition(Vec2(m_fBackSize - 20, 0));
				break;
			}
		}
		for (auto i = m_vcBonusItem.begin(); i != m_vcBonusItem.end(); i++)
		{
			if ((*i)->isVisible() == false)
				continue;
			if ((*i)->getPosition().x < -60)
				(*i)->setVisible(false);
		}
	}
	else
	{
		for (auto i = m_vcGameBackGround.begin(); i != m_vcGameBackGround.end(); i++)
		{
			if (CheckBackGroundRoof((*i)))
				break;
		}

		for (auto i = m_vcStageRoad.begin(); i != m_vcStageRoad.end(); i++)
		{
			if ((*i)->isVisible() == false)
				continue;
			if ((*i)->getPosition().x < -60)
				(*i)->setVisible(false);
		}
		for (auto i = m_vcEnemy.begin(); i != m_vcEnemy.end(); i++)
		{
			if ((*i)->isVisible() == false)
				continue;

			if ((*i)->getPosition().x < -60)
				(*i)->setVisible(false);
		}
		for (auto i = m_vcItem.begin(); i != m_vcItem.end(); i++)
		{
			if ((*i)->isVisible() == false)
				continue;

			if ((*i)->getPosition().x < -60)
				(*i)->setVisible(false);
		}
		for (auto i = m_vcPoint.begin(); i != m_vcPoint.end(); i++)
		{
			if ((*i)->isVisible() == false)
				continue;

			if ((*i)->getPosition().x < -60)
				(*i)->setVisible(false);
		}
	}
}
bool ObjectManager::CheckCrashRoad(cocos2d::Sprite* pPlayer)
{
	for (auto i = m_vcStageRoad.begin(); i != m_vcStageRoad.end(); i++)
	{
		if ((*i)->getBoundingBox().intersectsRect(pPlayer->getBoundingBox()) && (*i)->getPosition().y <= pPlayer->getBoundingBox().getMinY())
			return true;
	}
	return false;
}
bool ObjectManager::CheckGetItem(cocos2d::Sprite* pPlayer)
{
	if (m_bFever)
	{
		for (auto i = m_vcBonusItem.begin(); i != m_vcBonusItem.end(); i++)
		{
			if ((*i)->isVisible() == false)
				continue;
			if ((*i)->getBoundingBox().intersectsRect(pPlayer->getBoundingBox()))
			{
				(*i)->setVisible(false);
				pPlayer->setName((*i)->getName());
				GameManager::GetInstance()->SetJellyScore(1, true);
				return true;
			}
		}
	}
	else
	{
		for (auto i = m_vcItem.begin(); i != m_vcItem.end(); i++)
		{
			if ((*i)->isVisible() == false)
				continue;
			if ((*i)->getBoundingBox().intersectsRect(pPlayer->getBoundingBox()))
			{
				(*i)->setVisible(false);
				pPlayer->setName((*i)->getName());
				(*i)->setTag(0);
				return true;
			}
		}
	}
	return false;
}
bool ObjectManager::CheckGetJelly(cocos2d::Sprite* pPlayer)
{
	for (auto i = m_vcPoint.begin(); i != m_vcPoint.end(); i++)
	{
		if ((*i)->isVisible() == false)
			continue;
		if ((*i)->getBoundingBox().intersectsRect(pPlayer->getBoundingBox()))
		{
			(*i)->setVisible(false);
			pPlayer->setName((*i)->getName());
			(*i)->setTag(0);
			return true;
		}
	}
	return false;
}
bool ObjectManager::CheckCrashCookie(cocos2d::Sprite* pPlayer)
{
	for (auto i = m_vcEnemy.begin(); i != m_vcEnemy.end(); i++)
	{
		if ((*i)->isVisible() == false)
			continue;
		Rect rcEnemy = { (*i)->getBoundingBox().getMinX() + 30, (*i)->getBoundingBox().getMinY(), (*i)->getBoundingBox().getMaxX() - 90, (*i)->getBoundingBox().getMaxY() - 70};

		if (rcEnemy.intersectsRect(pPlayer->getBoundingBox()))
		{
			if (pPlayer->getScale() > 2.0f)
			{
				(*i)->setTag(0);
				MoveBy* pMoveBy = MoveBy::create(0.6f, Vec2(800, (*i)->getPosition().y + 40));
				Hide* pHide = Hide::create();
				Sequence* pSequence = Sequence::create(pMoveBy, pHide, nullptr);
				RotateBy* pRotateBy = RotateBy::create(0.5f, 360);
				RepeatForever* pRepeat = RepeatForever::create(pRotateBy);

				(*i)->runAction(pRepeat);
				(*i)->runAction(pSequence);

				return false;
			}
			else
				return true;
		}
	}
	return false;
}
bool ObjectManager::CheckCrashMove(cocos2d::Sprite* pPlayer)
{
	for (auto i = m_vcEnemy.begin(); i != m_vcEnemy.end(); i++)
	{
		if ((*i)->isVisible() == false)
			continue;
		Rect rcEnemy = { (*i)->getBoundingBox().getMinX() + 30, (*i)->getBoundingBox().getMinY(), (*i)->getBoundingBox().getMaxX() - 90, (*i)->getBoundingBox().getMaxY() - 70 };

		if (rcEnemy.intersectsRect(pPlayer->getBoundingBox()))
		{
			MoveBy* pMoveBy = MoveBy::create(0.6f, Vec2(800, (*i)->getPosition().y + 40));
			Hide* pHide = Hide::create();
			Sequence* pSequence = Sequence::create(pMoveBy, pHide, nullptr);
			RotateBy* pRotateBy = RotateBy::create(0.5f, 360);
			RepeatForever* pRepeat = RepeatForever::create(pRotateBy);

			(*i)->setTag(0);

			(*i)->runAction(pRepeat);
			(*i)->runAction(pSequence);

			return true;
		}
	}
	return false;
}
bool ObjectManager::CheckCrashDestroy(cocos2d::Sprite* pPlayer)
{
	for (auto i = m_vcEnemy.begin(); i != m_vcEnemy.end(); i++)
	{
		if ((*i)->isVisible() == false)
			continue;
		Rect rcEnemy = { (*i)->getBoundingBox().getMinX(), (*i)->getBoundingBox().getMinY(), (*i)->getBoundingBox().getMaxX(), (*i)->getBoundingBox().getMaxY() };

		if (rcEnemy.intersectsRect(pPlayer->getBoundingBox()))
		{
			(*i)->setPosition(Vec2(1000, 1000));
			(*i)->setVisible(false);
			return true;
		}
	}
	return false;
}
bool ObjectManager::CheckBackGroundRoof(Sprite* pBackGround)
{
	if (pBackGround->getPosition().x <= -m_fBackSize)
	{
		pBackGround->setPosition(Vec2(m_fBackSize - 20, 0));

		if (pBackGround->getTag() == 2 || pBackGround->getTag() == 3)
			return false;

		m_iStageRoofCount++;

		if (m_iStageRoofCount == m_iStageRoofMax)
		{
			int iLevel = (int)m_eGameStage;
			iLevel++;
			m_eGameStage = (GAME_STAGE)iLevel;
			m_iStageRoofCount = 0;
			ChangeSceneFade();
			return true;
		}

	}

	return false;
}

void ObjectManager::ChangeSceneFade()
{
	// IN true Out false

	int iCount = 0;
	for (auto i = m_vcGameBackGround.begin(); i != m_vcGameBackGround.end(); i++)
	{
		FadeOut* pFadeOut = FadeOut::create(2.0f);
		Hide* pHide = Hide::create();
		auto pChangeStage = CallFuncN::create(CC_CALLBACK_0(ObjectManager::ChangeBackGround, this));
		Sequence* pSequence;
		if(iCount == 0)
			pSequence = Sequence::create(pFadeOut, pHide, pChangeStage, nullptr);
		else
			pSequence = Sequence::create(pFadeOut, pHide, nullptr);
		(*i)->runAction(pSequence);

		iCount++;
	}
}
void ObjectManager::ChangeBackGround()
{
	m_vcGameBackGround.clear();

	if (m_eGameStage == GAME_STAGE_TWO)
	{
		m_iStageRoofMax = 14;
		for (auto i = m_vcBackGroundStage01.begin(); i != m_vcBackGroundStage01.end(); i++)
		{
			m_vcGameBackGround.pushBack((*i));
			(*i)->setVisible(true);
			(*i)->setOpacity(0);
			FadeIn* pFadeIn = FadeIn::create(2.0f);
			Sequence* pSequenceTwo = Sequence::create(pFadeIn, nullptr);
			(*i)->runAction(pSequenceTwo);
		}
	}
	else if (m_eGameStage == GAME_STAGE_THR)
	{
		m_iStageRoofMax = 30;
		for (auto i = m_vcBackGroundStage02.begin(); i != m_vcBackGroundStage02.end(); i++)
		{
			m_vcGameBackGround.pushBack((*i));
			(*i)->setVisible(true);
			(*i)->setOpacity(0);
			FadeIn* pFadeIn = FadeIn::create(2.0f);
			Sequence* pSequenceTwo = Sequence::create(pFadeIn, nullptr);
			(*i)->runAction(pSequenceTwo);
		}
	}
}


void ObjectManager::GenerateBonusJelly()
{
	int Random = rand() % 3 + 0;

	char str[100] = { 0 };
	sprintf(str, "shop_jelly_%02d.png", GameManager::GetInstance()->GetUserData()->iJellyLevel);
	Sprite* pJelly = Sprite::createWithSpriteFrameName(str);
	pJelly->setPosition(Vec2(ResManager::GetInstance()->GetDisplaySize().width, 450 - (Random * 100)));
	m_vcBonusItem.pushBack(pJelly);
	m_pScene->addChild(pJelly, 2);
}
void ObjectManager::ChangeFeverMode()
{
	m_bFever = true;
	
	auto pGenerateBouns = CallFuncN::create(CC_CALLBACK_0(ObjectManager::GenerateBonusJelly, this));
	auto pDelay = DelayTime::create(0.1f);
	auto pSequence = Sequence::create(pGenerateBouns, pDelay, nullptr);
	auto pRepeat = RepeatForever::create(pSequence);
	m_pLoadingCharacter->runAction(pRepeat);
	

	for (auto i = m_vcGameBackGround.begin(); i != m_vcGameBackGround.end(); i++)
	{
		FadeOut* pFadeOut = FadeOut::create(1.0f);
		Hide* pHide = Hide::create();
		Sequence* pSequence = Sequence::create(pFadeOut, pHide, nullptr);
		(*i)->runAction(pSequence);
	}
	for (auto i = m_vcStageRoad.begin(); i != m_vcStageRoad.end(); i++)
	{
		FadeOut* pFadeOut = FadeOut::create(1.0f);
		Hide* pHide = Hide::create();
		Sequence* pSequence = Sequence::create(pFadeOut, pHide, nullptr);
		(*i)->runAction(pSequence);
	}
	for (auto i = m_vcEnemy.begin(); i != m_vcEnemy.end(); i++)
	{
		FadeOut* pFadeOut = FadeOut::create(1.0f);
		Hide* pHide = Hide::create();
		Sequence* pSequence = Sequence::create(pFadeOut, pHide, nullptr);
		(*i)->runAction(pSequence);
	}
	for (auto i = m_vcPoint.begin(); i != m_vcPoint.end(); i++)
	{
		FadeOut* pFadeOut = FadeOut::create(1.0f);
		Hide* pHide = Hide::create();
		Sequence* pSequence = Sequence::create(pFadeOut, pHide, nullptr);
		(*i)->runAction(pSequence);
	}
	for (auto i = m_vcItem.begin(); i != m_vcItem.end(); i++)
	{
		FadeOut* pFadeOut = FadeOut::create(1.0f);
		Hide* pHide = Hide::create();
		Sequence* pSequence = Sequence::create(pFadeOut, pHide, nullptr);
		(*i)->runAction(pSequence);
	}
	for (auto i = m_vcBonusBackGround.begin(); i != m_vcBonusBackGround.end(); i++)
	{
		(*i)->setVisible(true);
		FadeIn* pFadeIn = FadeIn::create(2.0f);
		Sequence* pSequenceTwo = Sequence::create(pFadeIn, nullptr);
		(*i)->runAction(pSequenceTwo);
	}

}
void ObjectManager::UnChangeFeverMode()
{
	m_bFever = false;
	m_pLoadingCharacter->stopAllActions();
	for (auto i = m_vcBonusItem.begin(); i != m_vcBonusItem.end(); i++)
	{
		(*i)->setVisible(false);
	}
	m_vcBonusItem.clear();
	for (auto i = m_vcGameBackGround.begin(); i != m_vcGameBackGround.end(); i++)
	{
		FadeIn* pFadeIn = FadeIn::create(2.0f);
		Show* pShow = Show::create();
		Sequence* pSequence = Sequence::create(pShow, pFadeIn, nullptr);
		(*i)->runAction(pSequence);
	}
	for (auto i = m_vcStageRoad.begin(); i != m_vcStageRoad.end(); i++)
	{
		FadeIn* pFadeIn = FadeIn::create(2.0f);
		Show* pShow = Show::create();
		Sequence* pSequence = Sequence::create(pShow, pFadeIn, nullptr);
		(*i)->runAction(pSequence);
	}
	for (auto i = m_vcPoint.begin(); i != m_vcPoint.end(); i++)
	{
		if ((*i)->getTag() == 0)
			continue;
		FadeIn* pFadeIn = FadeIn::create(2.0f);
		Show* pShow = Show::create();
		Sequence* pSequence = Sequence::create(pShow, pFadeIn, nullptr);
		(*i)->runAction(pSequence);
	}
	for (auto i = m_vcEnemy.begin(); i != m_vcEnemy.end(); i++)
	{
		if ((*i)->getTag() == 0)
			continue;
		FadeIn* pFadeIn = FadeIn::create(2.0f);
		Show* pShow = Show::create();
		Sequence* pSequence = Sequence::create(pShow, pFadeIn, nullptr);
		(*i)->runAction(pSequence);
	}
	for (auto i = m_vcItem.begin(); i != m_vcItem.end(); i++)
	{
		if ((*i)->getTag() == 0)
			continue;
		FadeIn* pFadeIn = FadeIn::create(2.0f);
		Show* pShow = Show::create();
		Sequence* pSequence = Sequence::create(pShow, pFadeIn, nullptr);
		(*i)->runAction(pSequence);
	}
	for (auto i = m_vcBonusBackGround.begin(); i != m_vcBonusBackGround.end(); i++)
	{
		FadeOut* pFadeOut = FadeOut::create(1.0f);
		Hide* pHide = Hide::create();
		Sequence* pSequenceTwo = Sequence::create(pFadeOut, pHide, nullptr);
		(*i)->runAction(pSequenceTwo);
	}
}
void ObjectManager::ChangeMaxSpeed()
{
	m_iSpeed = BOOST_SPEED;
}
void ObjectManager::ChangeMinSpeed()
{
	m_iSpeed = BACK_SPEED;
}