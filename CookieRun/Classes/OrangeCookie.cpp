#include "OrangeCookie.h"
#include "GameManager.h"
#include "ResManager.h"

USING_NS_CC;

OrangeCookie::OrangeCookie()
{
}


OrangeCookie::~OrangeCookie()
{
}


void OrangeCookie::Init(cocos2d::Scene* pScene, cocos2d::SpriteFrameCache* pJumpSpriteFrame, ObjectManager* pObject)
{
	m_bGetBall = false;
	m_bAttack = false;
	m_bExplosion = false;
	m_bSpawnBall = false;
	iUpdataDelay = 0;

	m_pEffectCache = SpriteFrameCache::sharedSpriteFrameCache();
	m_pEffectCache->addSpriteFramesWithFile("character_02_effect.plist");

	m_pDropBall = Sprite::createWithSpriteFrameName("cookie_02_ball_00.png");
	m_pDropBall->setScale(2.0f);
	pScene->addChild(m_pDropBall, 2);

	m_pScene = pScene;
	m_pObjectManager = pObject;
	PlayerCookie::Init(pScene, pJumpSpriteFrame, pObject);
}
void OrangeCookie::StartGame()
{
	DelayTime* pDelay = DelayTime::create(8.0f);
	auto pSetGenerate = CallFuncN::create(CC_CALLBACK_0(OrangeCookie::GenerateBall, this));
	Sequence* pSequence = Sequence::create(pDelay, pSetGenerate, nullptr);
	RepeatForever* pRepeat = RepeatForever::create(pSequence);
	m_pDropBall->runAction(pRepeat);

	PlayerCookie::StartGame();
}
void OrangeCookie::Input()
{
	if (PlayerCookie::GetDubbleJump())
		return;
	if (PlayerCookie::GetSlide())
		return;
	if (m_bGetBall == false || PlayerCookie::GetJump() == false || PlayerCookie::GetFever())
	{
		PlayerCookie::Input();
		return;
	}

	PlayerCookie::GetSprite()->stopAllActions();

	char str[100] = { 0 };
	if (PlayerCookie::GetJump())
	{
		PlayerCookie::SetDubbleJump();
		PlayerCookie::SetJump();
		sprintf(str, "cookie_%02d_jump_skil_00.png", GameManager::GetInstance()->GetUserData()->iCharacter);
		PlayerCookie::GetSprite()->setDisplayFrame(PlayerCookie::GetCache()->getSpriteFrameByName(str));

		JumpBy* pJumpBy = JumpBy::create(0.7f, Vec2(0, -100), 200, 1);
		auto pSetJump = CallFuncN::create(CC_CALLBACK_0(PlayerCookie::SetRunAnimate, this));
		Sequence* pSequence = Sequence::create(pJumpBy, pSetJump, nullptr);
		PlayerCookie::GetSprite()->runAction(pSequence);
	}
}

void OrangeCookie::GenerateBall()
{
	if (m_bGetBall)
		return;
	if (PlayerCookie::GetFever())
		return;

	m_bSpawnBall = true;
	m_pDropBall->setPosition(Vec2(ResManager::GetInstance()->GetDisplaySize().width, 150));
}

void OrangeCookie::InputPush()
{
	if (m_bGetBall == false || PlayerCookie::GetDubbleJump() == false)
	{
		PlayerCookie::InputPush();
		return;
	}
	if (m_bAttack)
		return;
	if (m_bExplosion)
		return;

	m_bAttack = true;
	m_bGetBall = false;
	char str[100] = { 0 };
	Vector<SpriteFrame*> pAniFrames;
	for (int i = 1; i < 5; i++)
	{
		sprintf(str, "cookie_02_jump_skil_%02d.png", i);

		SpriteFrame* Frame = PlayerCookie::GetCache()->getSpriteFrameByName(str);
		pAniFrames.pushBack(Frame);
	}
	Animation* pAnimation = Animation::createWithSpriteFrames(pAniFrames, 0.1f);
	Animate* pAnimate = Animate::create(pAnimation);
	PlayerCookie::GetSprite()->runAction(pAnimate);

	m_pBall = Sprite::createWithSpriteFrameName("cookie_02_ball_00.png");
	m_pBall->setPosition(PlayerCookie::GetSprite()->getBoundingBox().getMaxX() + 50, PlayerCookie::GetSprite()->getBoundingBox().getMinY() + 20);
	m_pBall->setScale(1.5f);
	m_pScene->addChild(m_pBall, 3);

	MoveTo* pMoveTo = MoveTo::create(0.3f, Vec2(PlayerCookie::GetSprite()->getBoundingBox().getMaxX() + 230, 60));
	auto pExplosion = CallFuncN::create(CC_CALLBACK_0(OrangeCookie::ExplosionBall, this));
	Sequence* pSequence = Sequence::create(pMoveTo, pExplosion, nullptr);

	m_pBall->runAction(pSequence);
}
void OrangeCookie::UpdataCharacter()
{
	for (auto i = m_vcExplosionEffect.begin(); i != m_vcExplosionEffect.end(); i++)
	{
		if ((*i)->isVisible() == false)
			continue;

		(*i)->setPosition(Vec2((*i)->getPosition().x - m_pObjectManager->GetSpeed(), (*i)->getPosition().y));
		if (m_pObjectManager->CheckCrashDestroy((*i)))
		{
			GameManager::GetInstance()->SetJellyScore(30, true);
		}
	}

	UpdataDropBall();


	if (m_bAttack == false)
		return;

	iUpdataDelay++;

	if (iUpdataDelay % 2 != 0)
		return;

	Sprite* pFire = Sprite::createWithSpriteFrameName("cookie_02_ball_effect.png");
	pFire->setPosition(Vec2(m_pBall->getPosition().x - 20, m_pBall->getPosition().y + 20));
	pFire->setScale(1.5f);
	SetRotate(pFire, Vec2(PlayerCookie::GetSprite()->getBoundingBox().getMaxX() + 230, 60));
	m_pScene->addChild(pFire);
	m_vcEffect.pushBack(pFire);

	for (auto i = m_vcEffect.begin(); i != m_vcEffect.end(); i++)
	{
		if (m_pObjectManager->CheckCrashDestroy((*i)))
		{
			GameManager::GetInstance()->SetJellyScore(4, true);
		}
	}
}

void OrangeCookie::UpdataDropBall()
{
	if (m_bSpawnBall == false)
		return;

	if (m_pDropBall->getBoundingBox().intersectsRect(m_pObjectManager->GetPlayer()->getBoundingBox()))
	{
		m_pDropBall->setPosition(Vec2(1000, 1000));
		m_bGetBall = true;
	}

	m_pDropBall->setPosition(Vec2(m_pDropBall->getPosition().x - m_pObjectManager->GetSpeed(), m_pDropBall->getPosition().y));

	if (m_pDropBall->getPosition().x < -60)
		m_bSpawnBall = false;
}

void OrangeCookie::ExplosionBall()
{
	m_bAttack = false;
	m_bExplosion = true;
	m_pBall->setVisible(false);
	iUpdataDelay = 0;

	for (auto i = m_vcEffect.begin(); i != m_vcEffect.end(); i++)
	{
		(*i)->setVisible(false);
	}
	m_vcEffect.clear();

	for (int i = 0; i < 4; i++)
	{
		Sprite* pExplosion = Sprite::createWithSpriteFrameName("cookie_02_explosion_00.png");
		if (i % 2 == 0)
		{
			pExplosion->setPosition(Vec2((m_pBall->getPosition().x - 60) + (i * 60), m_pBall->getPosition().y + 85));
			pExplosion->setScale(3.0f);
		}
		else
		{
			pExplosion->setPosition(Vec2((m_pBall->getPosition().x - 60) + (i * 60), m_pBall->getPosition().y + 50));
			pExplosion->setScale(2.5f);
		}
		

		char str[100] = { 0 };
		Vector<SpriteFrame*> pAniFrames;
		for (int i = 0; i < 5; i++)
		{
			sprintf(str, "cookie_02_explosion_%02d.png", i);
			SpriteFrame* Frame = m_pEffectCache->getSpriteFrameByName(str);
			pAniFrames.pushBack(Frame);
		}
		for (int i = 3; i >= 0; i--)
		{
			sprintf(str, "cookie_02_explosion_%02d.png", i);
			SpriteFrame* Frame = m_pEffectCache->getSpriteFrameByName(str);
			pAniFrames.pushBack(Frame);
		}
		Animation* pAnimation = Animation::createWithSpriteFrames(pAniFrames, 0.13f);
		Animate* pAnimate = Animate::create(pAnimation);
		auto pDestroy = CallFuncN::create(CC_CALLBACK_0(OrangeCookie::DestroyEffect, this));
		Sequence* pSequence = Sequence::create(pAnimate, pDestroy, nullptr);
		pExplosion->runAction(pSequence);

		m_pScene->addChild(pExplosion, 3);
		m_vcExplosionEffect.pushBack(pExplosion);
	}
}
void OrangeCookie::SetRotate(Node* origin, Vec2 target)
{
	auto dVec2 = target - origin->getPosition();
	auto radian = atan2(dVec2.x, dVec2.y);
	auto degree = CC_RADIANS_TO_DEGREES(radian);

	origin->setRotation(degree);
}
void OrangeCookie::DestroyEffect()
{
	m_bExplosion = false;
	for (auto i = m_vcExplosionEffect.begin(); i != m_vcExplosionEffect.end(); i++)
	{
		(*i)->setVisible(false);
	}
	m_vcExplosionEffect.clear();
}