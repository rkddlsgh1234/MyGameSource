#include "Boss.h"
#include "ResManager.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;


USING_NS_CC;

Boss::Boss()
{
}


Boss::~Boss()
{
}
void Boss::Init(cocos2d::SpriteFrameCache* pBossCache, cocos2d::Scene* pScene)
{
	m_pBossCache = pBossCache;
	m_bSpawn = false;
	m_bPattern = false;
	m_bFall = false;
	m_bDie = false;
	m_iEnemy = 0;
	m_iHp = 30;
	m_pBoss = Sprite::createWithSpriteFrameName("boss_jump.png");
	m_pBoss->setPosition(Vec2(500, 1000));
	pScene->addChild(m_pBoss);
	m_pScene = pScene;
}

void Boss::SpawnBoss()
{
	if (m_bSpawn)
		return;

	m_pBoss->setDisplayFrame(m_pBossCache->getSpriteFrameByName("boss_step_00.png"));
	m_bSpawn = true;
	SetPattern();
}
void Boss::CheckTile()
{
	if (m_ePattern == BOSS_PATTERN_FALL && m_bFall)
	{
		m_pBoss->setDisplayFrame(m_pBossCache->getSpriteFrameByName("boss_step_00.png"));
		m_bFall = false;
	}
}
void Boss::SetTexture()
{
	m_pBoss->setDisplayFrame(m_pBossCache->getSpriteFrameByName("boss_step_00.png"));
}

void Boss::SetPattern()
{
	auto pAttack = CallFuncN::create(CC_CALLBACK_0(Boss::Attack, this));
	auto pFall = CallFuncN::create(CC_CALLBACK_0(Boss::Fall, this));
	auto pJump = CallFuncN::create(CC_CALLBACK_0(Boss::Jump, this));
	auto pDelayOne = DelayTime::create(4.0f);
	auto pDelayTwo = DelayTime::create(4.0f);
	auto pDelayThr = DelayTime::create(4.0f);
	auto pSequenece = Sequence::create(pAttack, pDelayOne, pFall, pDelayTwo, pJump, pDelayThr, nullptr);
	auto pRepeat = RepeatForever::create(pSequenece);
	pRepeat->setTag(1);
	m_pBoss->runAction(pRepeat);
}
void Boss::Jump()
{
	m_ePattern = BOSS_PATTERN_JUMP;
	m_pBoss->setDisplayFrame(m_pBossCache->getSpriteFrameByName("boss_jump.png"));
	auto pSetTexture = CallFuncN::create(CC_CALLBACK_0(Boss::SetTexture, this));
	auto pJumpBy = JumpBy::create(0.8f, Vec2(0, 220), 300, 1);
	auto pSequenece = Sequence::create(pJumpBy, pSetTexture, nullptr);
	m_pBoss->runAction(pSequenece);
}
void Boss::Attack()
{
	m_ePattern = BOSS_PATTERN_ATTACK;
	m_iEnemy = 2;

	Vector<SpriteFrame*> pAniFrames;
	char str[100] = { 0 };
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			sprintf(str, "boss_attack_%02d.png", j);
			SpriteFrame* Frame = m_pBossCache->getSpriteFrameByName(str);
			pAniFrames.pushBack(Frame);
		}
	}
	SpriteFrame* Frame = m_pBossCache->getSpriteFrameByName("boss_attack_00.png");
	pAniFrames.pushBack(Frame);

	Animation* pAnimation = Animation::createWithSpriteFrames(pAniFrames, 0.3f);
	Animate* pAnimate = Animate::create(pAnimation);
	m_pBoss->runAction(pAnimate);
}
void Boss::Fall()
{
	m_ePattern = BOSS_PATTERN_FALL;
	m_bPattern = true;
	m_bFall = true;
	m_pBoss->setPosition(Vec2(m_pBoss->getPosition().x, m_pBoss->getPosition().y - 5));
	m_pBoss->setDisplayFrame(m_pBossCache->getSpriteFrameByName("boss_jump.png"));
}

void Boss::RemoveEnemyIndex()
{
	m_iEnemy--;
}
void Boss::SetDamage(int oDamage)
{
	m_iHp -= oDamage;
	m_pBoss->setOpacity(155);
	auto pHitEffect = CallFuncN::create(CC_CALLBACK_0(Boss::SetHitEffect, this));
	auto pDelayTime = DelayTime::create(0.02f);
	auto pSequence = Sequence::create(pDelayTime , pHitEffect, nullptr);
	m_pBoss->runAction(pSequence);
	SimpleAudioEngine::getInstance()->playEffect("sound/fx_boss_body_01.wav");
}

void Boss::SetHitEffect()
{
	m_pBoss->setOpacity(255);
}
void Boss::SetDead()
{
	m_bDie = false;
	m_pBoss->setOpacity(255);
	m_pBoss->stopAllActions();
	Vector<SpriteFrame*> pAniFrames;
	char str[100] = { 0 };

	for (int j = 0; j < 3; j++)
	{
		sprintf(str, "boss_die_%02d.png", j);
		SpriteFrame* Frame = m_pBossCache->getSpriteFrameByName(str);
		pAniFrames.pushBack(Frame);
	}


	Animation* pAnimation = Animation::createWithSpriteFrames(pAniFrames, 1.0f);
	Animate* pAnimate = Animate::create(pAnimation);
	m_pBoss->runAction(pAnimate);
	SimpleAudioEngine::getInstance()->playEffect("sound/fx_boss_death_01.wav");
}