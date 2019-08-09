#include "PeachCookie.h"
#include "ResManager.h"
#include "GameManager.h"

USING_NS_CC;

PeachCookie::PeachCookie()
{
}


PeachCookie::~PeachCookie()
{
}

void PeachCookie::Init(cocos2d::Scene* pScene, cocos2d::SpriteFrameCache* pJumpSpriteFrame, ObjectManager* pObject)
{
	m_bSkil = false;

	m_pScene = pScene;
	m_pObjectManager = pObject;
	PlayerCookie::Init(pScene, pJumpSpriteFrame, pObject);

	m_pDelay = Sprite::create("shop_show_coin.png");
	m_pScene->addChild(m_pDelay, 0);

	for (int i = 0; i < 2; i++)
	{
		Sprite* pBackEffect = Sprite::create("cookie_03_effect_bg.png");
		ResManager::GetInstance()->SetFullSizeSprite(pBackEffect);
		if (i == 1)
			pBackEffect->setPosition(Vec2(ResManager::GetInstance()->GetDisplaySize().width, ResManager::GetInstance()->GetDisplaySize().height));
		else
			pBackEffect->setPosition(Vec2(0, ResManager::GetInstance()->GetDisplaySize().height));

		m_vcBackEffect.pushBack(pBackEffect);
		m_pScene->addChild(pBackEffect, 1);
	}
}

void PeachCookie::Input()
{
	if (m_bSkil == false || PlayerCookie::GetFever())
	{
		PlayerCookie::Input();
		return;
	}
	if (PlayerCookie::GetDubbleJump())
		return;

	PlayerCookie::GetSprite()->stopAllActions();

	char str[100] = { 0 };
	if (PlayerCookie::GetJump())
	{
		PlayerCookie::SetDubbleJump();
		Vector<SpriteFrame*> pAniFrames;

		for (int i = 1; i < 5; i++)
		{
			sprintf(str, "cookie_03_jump_skil_%02d.png", i);

			SpriteFrame* Frame = PlayerCookie::GetCache()->getSpriteFrameByName(str);
			pAniFrames.pushBack(Frame);
		}
		SpriteFrame* Frame;
		Frame = PlayerCookie::GetCache()->getSpriteFrameByName("cookie_03_jump01.png");
		pAniFrames.pushBack(Frame);

		Animation* pAnimation = Animation::createWithSpriteFrames(pAniFrames, 0.12f);
		Animate* pAnimate = Animate::create(pAnimation);
		PlayerCookie::GetSprite()->runAction(pAnimate);

		JumpBy* pJumpBy = JumpBy::create(0.7f, Vec2(0, -100), 200, 1);
		auto pSetJump = CallFuncN::create(CC_CALLBACK_0(PeachCookie::SetRunAni, this));
		Sequence* pSequence = Sequence::create(pJumpBy, pSetJump, nullptr);
		PlayerCookie::GetSprite()->runAction(pSequence);
	}
	else
	{
		PlayerCookie::SetJump();
		PlayerCookie::GetSprite()->setDisplayFrame(PlayerCookie::GetCache()->getSpriteFrameByName("cookie_03_jump_skil_01.png"));

		JumpBy* pJumpBy = JumpBy::create(0.5f, Vec2(0, 0), 150, 1);
		auto pSetJump = CallFuncN::create(CC_CALLBACK_0(PeachCookie::SetRunAni, this));
		Sequence* pSequence = Sequence::create(pJumpBy, pSetJump, nullptr);
		PlayerCookie::GetSprite()->runAction(pSequence);
	}
}
void PeachCookie::UpdataCharacter()
{
	if (m_bSkil == false)
		return;
	for (auto i = m_vcBackEffect.begin(); i != m_vcBackEffect.end(); i++)
	{
		(*i)->setPosition(Vec2((*i)->getPosition().x - m_pObjectManager->GetSpeed(), (*i)->getPosition().y));

		if ((*i)->getPosition().x < -(ResManager::GetInstance()->GetDisplaySize().width))
			(*i)->setPosition(Vec2(ResManager::GetInstance()->GetDisplaySize().width, (*i)->getPosition().y));
	}
	return;
}
void PeachCookie::StartGame()
{
	DelayTime* pDelay = DelayTime::create(8.0f);
	auto pSetSkil = CallFuncN::create(CC_CALLBACK_0(PeachCookie::SetSkil, this));
	Sequence* pSequence = Sequence::create(pDelay, pSetSkil, nullptr);
	m_pDelay->runAction(pSequence);
	PlayerCookie::StartGame();
}

void PeachCookie::SetSkil()
{
	if (PlayerCookie::GetFever())
		return;

	for (auto i = m_vcBackEffect.begin(); i != m_vcBackEffect.end(); i++)
	{
		auto pMoveTo = MoveTo::create(0.3f, Vec2((*i)->getPosition().x, 0));
		(*i)->runAction(pMoveTo);
	}

	m_bSkil = true;
	SetRunAni();
	DelayTime* pDelay = DelayTime::create(4.0f);
	auto pSetSkil = CallFuncN::create(CC_CALLBACK_0(PeachCookie::ResetSkil, this));
	Sequence* pSequence = Sequence::create(pDelay, pSetSkil, nullptr);
	m_pDelay->runAction(pSequence);
}
void PeachCookie::ResetSkil()
{
	for (auto i = m_vcBackEffect.begin(); i != m_vcBackEffect.end(); i++)
	{
		auto pMoveTo = MoveTo::create(0.3f, Vec2((*i)->getPosition().x, ResManager::GetInstance()->GetDisplaySize().height));
		(*i)->runAction(pMoveTo);
	}

	PlayerCookie::SetRunAnimate();
	m_bSkil = false;
	DelayTime* pDelay = DelayTime::create(8.0f);
	auto pSetSkil = CallFuncN::create(CC_CALLBACK_0(PeachCookie::SetSkil, this));
	Sequence* pSequence = Sequence::create(pDelay, pSetSkil, nullptr);
	m_pDelay->runAction(pSequence);
}

void PeachCookie::SetRunAni()
{
	PlayerCookie::ResetState();
	
	Vector<SpriteFrame*> pAniFrames;
	char str[100] = { 0 };
	for (int i = 0; i < 4; i++)
	{
		sprintf(str, "cookie_03_run_skil_%02d.png", i);

		SpriteFrame* Frame = PlayerCookie::GetCache()->getSpriteFrameByName(str);
		pAniFrames.pushBack(Frame);
	}
	Animation* pAnimation = Animation::createWithSpriteFrames(pAniFrames, 0.05f);
	Animate* pAnimate = Animate::create(pAnimation);
	RepeatForever* pRepeatForever = RepeatForever::create(pAnimate);
	PlayerCookie::GetSprite()->runAction(pRepeatForever);
}

bool PeachCookie::CheckCrash()
{
	if (PlayerCookie::GetGodMode())
		return false;

	if (m_bSkil)
	{
		if (m_pObjectManager->CheckCrashMove(PlayerCookie::GetSprite()))
		{
			GameManager::GetInstance()->SetJellyScore(10, true);
			return false;
		}
	}
	else
		return PlayerCookie::CheckCrash();

	return false;
}