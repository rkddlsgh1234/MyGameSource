#include "Enemy.h"
#include "ResManager.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

USING_NS_CC;

Enemy::Enemy()
{
}


Enemy::~Enemy()
{
}

void Enemy::Init(cocos2d::Scene* pScene, cocos2d::SpriteFrameCache* pIceCache, cocos2d::SpriteFrameCache* pMobCache, int iMonsterNum)
{
	m_pIceCache = pIceCache;
	m_pMobCache = pMobCache;

	m_pDelay = Sprite::createWithSpriteFrameName("ice_00.png");
	pScene->addChild(m_pDelay, 0);

	m_pScene = pScene;

	m_eEnemyState = ENEMY_STATE_RUN;
	m_iHit = 0;
	m_iMonsterNum = iMonsterNum;
	m_bHit = false;
	m_bRun = false;
	m_bIceRun = false;
	m_bRotateRun = false;
	m_bDie = false;
	m_bExplosion = false;
	m_bItem = false;
}
void Enemy::InitEnemy(Vec2 VecPos, cocos2d::EventKeyboard::KeyCode keyCode)
{
	char str[100] = { 0 };
	sprintf(str, "mob%02d_left_run_00.png", m_iMonsterNum);
	m_pEnemy = Sprite::createWithSpriteFrameName(str);
	m_pEnemy->setPosition(VecPos);

	m_pIce = Sprite::createWithSpriteFrameName("ice_00.png");
	m_pIce->setAnchorPoint(Vec2(0, 0));
	m_pIce->setPosition(Vec2(m_pEnemy->getBoundingBox().getMinX() + 5, m_pEnemy->getBoundingBox().getMinY()));
	m_pIce->setVisible(false);

	m_MobWay = keyCode;
	m_pScene->addChild(m_pEnemy, 5);
	m_pScene->addChild(m_pIce, 5);

	MoveRepeat();
}

void Enemy::CountHit(cocos2d::EventKeyboard::KeyCode keyCode)
{
	if (m_bDie)
		return;

	DelayUnIce();
	if (m_iHit == 4)
	{
		m_bRotateRun = true;
		m_bHit = false;
		m_bIceRun = false;
		m_eEnemyState = ENEMY_STATE_ICE_RUN;
		RotateIce();
		m_MobWay = keyCode;
		return;
	}
	if (m_bHit == false)
	{
		m_eEnemyState = ENEMY_STATE_STACK_ICE;
		m_bRun = false;
		m_bHit = true;
		m_pEnemy->stopAllActions();
		m_pIce->setVisible(true);
		SetIceAnimation();
	}

	SetIceTexture();
	m_iHit++;
	if (m_iHit == 4)
	{
		m_eEnemyState = ENEMY_STATE_ICE;
		m_pEnemy->stopAllActions();
		m_pEnemy->setVisible(false);
		char str[100] = { 0 };
		sprintf(str, "mob%02d_left_ice.png", m_iMonsterNum);
		m_pEnemy->setDisplayFrame(m_pMobCache->getSpriteFrameByName(str));
	}
}
void Enemy::UnCountHit()
{
	if (m_bDie)
		return;

	m_iHit--;

	if (m_iHit == -1)
	{
		m_eEnemyState = ENEMY_STATE_RUN;
		m_iHit = 0;
		m_bHit = false;
		m_pIce->stopActionByTag(10);
		m_pIce->setVisible(false);
		m_pEnemy->stopAllActions();
		SetRunAnimation();
	}
	else if(m_iHit != 3)
	{
		m_pEnemy->setVisible(true);
		DelayUnIce();
		SetIceTexture();
		SetIceAnimation();
	}
}
void Enemy::SetIceTexture()
{
	if (m_bDie)
		return;
	char str[100] = { 0 };
	sprintf(str, "ice_%02d.png", m_iHit);
	m_pIce->setDisplayFrame(m_pIceCache->getSpriteFrameByName(str));
}
void Enemy::DelayUnIce()
{
	if (m_bDie)
		return;
	if (m_bRotateRun)
		return;
	if (m_bHit)
		m_pIce->stopAllActions();

	auto pDelayTime = DelayTime::create(2.0f);
	auto pUnCountHit = CallFuncN::create(CC_CALLBACK_0(Enemy::UnCountHit, this));
	auto pSequence = Sequence::create(pDelayTime, pUnCountHit, nullptr);
	pSequence->setTag(10);
	m_pIce->runAction(pSequence);
}

void Enemy::SetIceAnimation()
{
	if (m_bDie)
		return;
	if (m_bRotateRun)
		return;
	char str[100] = { 0 };
	Vector<SpriteFrame*> pAniFrames;
	if (m_MobWay == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
	{
		for (int i = 0; i < 2; i++)
		{
			sprintf(str, "mob%02d_left_ice_%02d.png", m_iMonsterNum,i);
			SpriteFrame* Frame = m_pMobCache->getSpriteFrameByName(str);
			pAniFrames.pushBack(Frame);
		}
	}
	else if (m_MobWay == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
	{
		for (int i = 0; i < 2; i++)
		{
			sprintf(str, "mob%02d_right_ice_%02d.png", m_iMonsterNum, i);
			SpriteFrame* Frame = m_pMobCache->getSpriteFrameByName(str);
			pAniFrames.pushBack(Frame);
		}
	}
	Animation* pAnimation = Animation::createWithSpriteFrames(pAniFrames, 0.1f);
	Animate* pAnimate = Animate::create(pAnimation);
	auto pRepeat = RepeatForever::create(pAnimate);
	m_pEnemy->runAction(pRepeat);
	pRepeat->setTag(0);
}

void Enemy::MoveEnemy()
{
	if (m_bDie)
		return;
	if (m_bHit)
		return;

	if (m_bRotateRun)
		FastMove();
	else
		SlowMove();
}
void Enemy::FastMove()
{
	if (m_bDie)
		return;
	if (m_bExplosion)
		return;

	if (m_pEnemy->getPosition().x - 10 <= 30 && m_MobWay == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
	{
		if (m_pEnemy->getPosition().y < 150)
			EffectExplosion();
		m_MobWay = EventKeyboard::KeyCode::KEY_RIGHT_ARROW;
	}
	else if (m_pEnemy->getPosition().x + 10 >= ResManager::GetInstance()->GetDisplaySize().width - 20 && m_MobWay == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
	{
		if (m_pEnemy->getPosition().y < 150)
			EffectExplosion();
		m_MobWay = EventKeyboard::KeyCode::KEY_LEFT_ARROW;
	}

	if (m_MobWay == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
	{
		m_pEnemy->setPosition(Vec2(m_pEnemy->getPosition().x - 10, m_pEnemy->getPosition().y));
	}
	else if (m_MobWay == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
	{
		m_pEnemy->setPosition(Vec2(m_pEnemy->getPosition().x + 10, m_pEnemy->getPosition().y));
	}
	m_pIce->setPosition(Vec2(m_pEnemy->getBoundingBox().getMinX() + 5, m_pEnemy->getBoundingBox().getMinY()));
}
void Enemy::SlowMove()
{
	if (m_bDie)
		return;
	if (m_pEnemy->getPosition().x - 1 <= 30 && m_MobWay == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
		return;
	else if (m_pEnemy->getPosition().x + 1 >= ResManager::GetInstance()->GetDisplaySize().width - 20 && m_MobWay == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
		return;

	if (m_MobWay == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
	{
		m_pEnemy->setPosition(Vec2(m_pEnemy->getPosition().x - 1, m_pEnemy->getPosition().y));
	}
	else if (m_MobWay == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
	{
		m_pEnemy->setPosition(Vec2(m_pEnemy->getPosition().x + 1, m_pEnemy->getPosition().y));
	}
	m_pIce->setPosition(Vec2(m_pEnemy->getBoundingBox().getMinX() + 5, m_pEnemy->getBoundingBox().getMinY()));
}

void Enemy::EffectExplosion()
{
	m_bExplosion = true;
	m_bDie = true;
	char str[100] = { 0 };
	Vector<SpriteFrame*> pAniFrames;

	for (int i = 0; i < 13; i++)
	{
		sprintf(str, "ice_effect_%02d.png", i);
		SpriteFrame* Frame = m_pMobCache->getSpriteFrameByName(str);
		pAniFrames.pushBack(Frame);
	}
	Animation* pAnimation = Animation::createWithSpriteFrames(pAniFrames, 0.07f);
	Animate* pAnimate = Animate::create(pAnimation);
	auto pDestroyEnemy = CallFuncN::create(CC_CALLBACK_0(Enemy::DestroyEnemy, this));
	auto pSequence = Sequence::create(pAnimate, pDestroyEnemy, nullptr);
	m_pIce->runAction(pSequence);
	m_pIce->setPosition(Vec2(m_pIce->getPosition().x, m_pIce->getPosition().y));

	SimpleAudioEngine::getInstance()->playEffect("sound/fx_death_00_01.wav");
}

void Enemy::MoveRepeat()
{
	if (m_bHit)
		return;
	if (m_bRotateRun)
		return;

	int iRandom = rand() % 5 + 2;

	if (m_MobWay == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
		m_MobWay = EventKeyboard::KeyCode::KEY_RIGHT_ARROW;
	else if (m_MobWay == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
		m_MobWay = EventKeyboard::KeyCode::KEY_LEFT_ARROW;

	SetRunAnimation();

	auto pDelay = DelayTime::create(iRandom);
	auto pChangeWay = CallFuncN::create(CC_CALLBACK_0(Enemy::MoveRepeat, this));
	auto pSequence = Sequence::create(pDelay, pChangeWay, nullptr);
	pSequence->setTag(1);
	m_pDelay->runAction(pSequence);
}
void Enemy::SetRunAnimation()
{
	if (m_bDie)
		return;
	if (m_bRotateRun)
		return;
	if (m_bRun)
		m_pEnemy->stopAllActions();

	m_bRun = true;

	char str[100] = { 0 };
	Vector<SpriteFrame*> pAniFrames;
	if (m_MobWay == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
	{
		for (int i = 0; i < 4; i++)
		{
			sprintf(str, "mob%02d_left_run_%02d.png", m_iMonsterNum, i);
			SpriteFrame* Frame = m_pMobCache->getSpriteFrameByName(str);
			pAniFrames.pushBack(Frame);
		}
	}
	else if (m_MobWay == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
	{
		for (int i = 0; i < 4; i++)
		{
			sprintf(str, "mob%02d_right_run_%02d.png", m_iMonsterNum, i);
			SpriteFrame* Frame = m_pMobCache->getSpriteFrameByName(str);
			pAniFrames.pushBack(Frame);
		}
	}
	Animation* pAnimation = Animation::createWithSpriteFrames(pAniFrames, 0.2f);
	Animate* pAnimate = Animate::create(pAnimation);
	auto pRepeat = RepeatForever::create(pAnimate);
	m_pEnemy->runAction(pRepeat);
}

void Enemy::ResetHit()
{
	if (m_bDie)
		return;
	SetIceAnimation();
}

void Enemy::MoveIce(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Sprite * pPlayer)
{
	if (m_bDie)
		return;
	if (m_bRotateRun)
		return;
	if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
	{
		if (m_pEnemy->getPosition().x - 3 <= 20)
			return;

		float fSize = m_pEnemy->getPosition().x + m_pEnemy->getContentSize().width - 5;
		if (fSize >= pPlayer->getBoundingBox().getMaxX())
			return;

		RotateIce();
		m_bIceRun = true;
		m_pEnemy->setPosition(Vec2(m_pEnemy->getPosition().x - 3, m_pEnemy->getPosition().y));
		m_pIce->setPosition(Vec2(m_pIce->getPosition().x - 3, m_pIce->getPosition().y));
	}
	else if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
	{
		if (m_pEnemy->getPosition().x + 3 >= ResManager::GetInstance()->GetDisplaySize().width - 20)
			return;
		float fSize = m_pEnemy->getPosition().x + 5;
		if (fSize <= pPlayer->getBoundingBox().getMinX())
			return;

		RotateIce();
		m_bIceRun = true;
		m_pEnemy->setPosition(Vec2(m_pEnemy->getPosition().x + 3, m_pEnemy->getPosition().y));
		m_pIce->setPosition(Vec2(m_pIce->getPosition().x + 3, m_pIce->getPosition().y));
	}
}

void Enemy::RotateIce()
{
	if (m_bDie)
		return;
	if (m_bIceRun)
		return;
	char str[100] = { 0 };
	Vector<SpriteFrame*> pAniFrames;
	for (int i = 0; i < 4; i++)
	{
		sprintf(str, "ice_rotate_%02d.png", i);
		SpriteFrame* Frame = m_pMobCache->getSpriteFrameByName(str);
		pAniFrames.pushBack(Frame);
	}
	Animation* pAnimation = Animation::createWithSpriteFrames(pAniFrames, 0.05f);
	Animate* pAnimate = Animate::create(pAnimation);
	auto pRepeat = RepeatForever::create(pAnimate);
	pRepeat->setTag(15);
	m_pIce->runAction(pRepeat);
}

void Enemy::ResetIceRun()
{
	if (m_bDie)
		return;
	m_bIceRun = false;
}

void Enemy::ReleasePush()
{
	if (m_bDie)
		return;
	if (m_bRotateRun)
		return;
	if (m_bIceRun)
		m_pIce->stopActionByTag(15);
	m_bIceRun = false;
}

void Enemy::DestroyEnemy()
{
	m_pEnemy->setVisible(false);
	m_pIce->setVisible(false);
	m_pDelay->setVisible(false);
	
	if (m_bItem)
		m_pItem->setVisible(false);
}

void Enemy::DamageIce()
{
	if (m_bDie)
		return;
	if (m_pEnemy->isVisible() == false)
		return;

	m_bDie = true;

	m_EnemyVec = m_pEnemy->getPosition();
	m_pIce->stopAllActions();
	m_pEnemy->stopAllActions();

	char str[100] = { 0 };
	Vector<SpriteFrame*> pAniFrames;
	sprintf(str, "mob%02d_damage_00.png", m_iMonsterNum);
	SpriteFrame* Frame = m_pMobCache->getSpriteFrameByName(str);
	pAniFrames.pushBack(Frame);

	for (int i = 0; i < 2; i++)
	{
		for (int j = 1; j < 3; j++)
		{
			sprintf(str, "mob%02d_damage_%02d.png", m_iMonsterNum, j);
			SpriteFrame* Frame = m_pMobCache->getSpriteFrameByName(str);
			pAniFrames.pushBack(Frame);
		}
	}
	for (int i = 0; i < 3; i++)
	{
		for (int j = 3; j < 7; j++)
		{
			sprintf(str, "mob%02d_damage_%02d.png", m_iMonsterNum, j);
			SpriteFrame* Frame = m_pMobCache->getSpriteFrameByName(str);
			pAniFrames.pushBack(Frame);
		}
	}
	Animation* pAnimation = Animation::createWithSpriteFrames(pAniFrames, 0.15f);
	Animate* pAnimate = Animate::create(pAnimation);
	m_pEnemy->runAction(pAnimate);

	int iRandY = 600;
	int iRandX;
	int iPosX;

	if (ResManager::GetInstance()->GetDisplaySize().width / 2 <= m_pEnemy->getPosition().x)
	{
		iRandX = 100;
		iPosX = 500;
	}
	else
	{
		iRandX = 500;
		iPosX = 100;
	}

	Vector<SpriteFrame*> pAniDead;
	for (int j = 7; j < 10; j++)
	{
		sprintf(str, "mob%02d_damage_%02d.png", m_iMonsterNum, j);
		SpriteFrame* Frame = m_pMobCache->getSpriteFrameByName(str);
		pAniDead.pushBack(Frame);
	}

	Animation* pDeadAnimation = Animation::createWithSpriteFrames(pAniDead, 0.3f);
	Animate* pDeadAnimate = Animate::create(pDeadAnimation);

	auto pMoveToOne = MoveTo::create(0.8f, Vec2(iRandX, iRandY));
	auto pMoveToTwo = MoveTo::create(0.5f, Vec2(ResManager::GetInstance()->GetDisplaySize().width / 2, iRandY + 80));
	auto pMoveToThr = MoveTo::create(0.5f, Vec2(iPosX, iRandY));
	auto pMoveToFor = MoveTo::create(0.8f, Vec2(m_EnemyVec.x, m_EnemyVec.y - 15));
	auto pHide = Hide::create();
	auto pDropItem = CallFuncN::create(CC_CALLBACK_0(Enemy::DropItem, this));
	auto pSequence = Sequence::create(pMoveToOne, pMoveToTwo, pMoveToThr, pMoveToFor, pDeadAnimate, pHide, pDropItem, nullptr);
	m_pEnemy->runAction(pSequence);
}

void Enemy::ChangeWay()
{
	if (m_MobWay == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
		m_MobWay = EventKeyboard::KeyCode::KEY_RIGHT_ARROW;
	else if (m_MobWay == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
		m_MobWay = EventKeyboard::KeyCode::KEY_LEFT_ARROW;

	if (m_eEnemyState == ENEMY_STATE_RUN)
		SetRunAnimation();
}
void Enemy::DropItem()
{
	m_bItem = true;
	int iRandom = rand() % 3;

	char str[100] = { 0 };

	if (iRandom == 0)
		sprintf(str, "item_01.png");
	else if (iRandom == 1)
		sprintf(str, "item_02.png");
	else if (iRandom == 2)
		sprintf(str, "item_03.png");

	m_pItem = Sprite::createWithSpriteFrameName(str);
	m_pItem->setPosition(m_pEnemy->getPosition().x, m_pEnemy->getPosition().y);
	m_pScene->addChild(m_pItem);
	m_pItem->setName(str);
}

void Enemy::SetIceRush(cocos2d::EventKeyboard::KeyCode keyCode)
{
	if (m_bRotateRun)
		return;
	m_bRotateRun = true;
	m_bHit = false;
	m_bIceRun = false;
	m_eEnemyState = ENEMY_STATE_ICE_RUN;
	RotateIce();
	m_MobWay = keyCode;
}

void Enemy::DestroyIce()
{
	m_pIce->setVisible(false);
}