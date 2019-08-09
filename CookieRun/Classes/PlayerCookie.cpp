#include "PlayerCookie.h"

#include "ResManager.h"
#include "GameManager.h"

USING_NS_CC;

PlayerCookie::PlayerCookie()
{
}


PlayerCookie::~PlayerCookie()
{

}

void PlayerCookie::Init(cocos2d::Scene* pScene, cocos2d::SpriteFrameCache* pJumpSpriteFrame, ObjectManager* pObject)
{
	char str[100] = { 0 };
	sprintf(str, "cookie_%02d_jump00.png", GameManager::GetInstance()->GetUserData()->iCharacter);
	m_pPlayer = Sprite::createWithSpriteFrameName(str);
	m_pEffectCache = SpriteFrameCache::sharedSpriteFrameCache();
	m_pEffectCache->addSpriteFramesWithFile("effect.plist");

	m_bJump = false;
	m_bDubbleJump = false;
	m_bGod = false;
	m_bSlide = false;
	m_bDie = false;
	m_bFeverAction = false;
	m_bItemBoost = false;
	m_bPush = false;
	m_bItemBig = false;
	m_bFever = false;
	m_pCharacterCache = pJumpSpriteFrame;
	m_pPlayer->setPosition(Vec2(140, 115));
	m_pPlayer->setScale(2.0f);
	m_pPlayer->setVisible(false);
	m_pScene = pScene;
	pScene->addChild(m_pPlayer, 2);
	SetRunAnimate();
	m_pObjectManager = pObject;
}

bool PlayerCookie::Updata(ObjectManager* pObject)
{
	UpdataItem();
	if (SetGravity(pObject))
		return true;

	return false;
}
void PlayerCookie::StartGame()
{
	m_pPlayer->setVisible(true);
	m_bControl = true;
}

void PlayerCookie::UpdataCharacter()
{
	return;
}
bool PlayerCookie::SetGravity(ObjectManager* pObject)
{
	if (m_bJump)
		return false;
	if (m_bItemBoost)
		return false;

	if (m_bFever)
	{
		if (m_bFeverAction)
			return false;

		char str[100] = { 0 };
		sprintf(str, "cookie_%02d_jump00.png", GameManager::GetInstance()->GetUserData()->iCharacter);
		m_pPlayer->setDisplayFrame(m_pCharacterCache->getSpriteFrameByName(str));
		if (m_bPush)
		{
			if (m_pPlayer->getBoundingBox().getMinY() + 3 >= 0)
				m_pPlayer->setPosition(Vec2(m_pPlayer->getPosition().x, m_pPlayer->getPosition().y + 6));
		}
		else
		{
			if (m_pPlayer->getBoundingBox().getMinY() - 6 >= 150)
				m_pPlayer->setPosition(Vec2(m_pPlayer->getPosition().x, m_pPlayer->getPosition().y - 6));
		}
	}
	else if (pObject->CheckCrashRoad(m_pPlayer) == false && m_bDie == false)
	{
		if (m_bSlide)
		{
			MoveBy* pMoveBy = MoveBy::create(0.2f, Vec2(0, -200));
			m_pPlayer->runAction(pMoveBy);
		}
		else
			m_pPlayer->setPosition(Vec2(m_pPlayer->getPosition().x, m_pPlayer->getPosition().y - 10));
	}

	if (m_bControl == false)
	{
		if (pObject->CheckCrashRoad(m_pPlayer))
		{
			m_bControl = true;
			SetRunAnimate();
		}
	}

	if (m_bFever == false)
	{
		if (m_pPlayer->getPosition().y < -50)
		{
			m_bDie = true;
			return true;
		}
	}
	return false;
}
void PlayerCookie::UpdataItem()
{
	if (m_bItemBig)
		m_pPlayer->setScale(4.0f);
	else
		m_pPlayer->setScale(2.0f);

	if (m_bItemBoost)
	{
		Sprite* pEffect = Sprite::createWithSpriteFrameName("effect_boost.png");
		pEffect->setPosition(Vec2(m_pPlayer->getPosition().x, m_pPlayer->getPosition().y));
		m_vcEffect.pushBack(pEffect);
		m_pScene->addChild(pEffect, 0);
	}

	for (auto i = m_vcEffect.begin(); i != m_vcEffect.end(); i++)
	{
		if ((*i)->isVisible() == false)
			continue;

		if ((*i)->getPosition().x < -50)
			(*i)->setVisible(false);

		(*i)->setPosition(Vec2((*i)->getPosition().x - BOOST_SPEED, (*i)->getPosition().y));
	}
	for (auto i = m_vcCoinEffect.begin(); i != m_vcCoinEffect.end(); i++)
	{
		if ((*i)->isVisible() == false)
			continue;

		if ((*i)->getPosition().x < -50)
			(*i)->setVisible(false);

		if(m_bItemBoost)
			(*i)->setPosition(Vec2((*i)->getPosition().x - BOOST_SPEED, (*i)->getPosition().y));
		else
			(*i)->setPosition(Vec2((*i)->getPosition().x - BACK_SPEED, (*i)->getPosition().y));
	}
}

void PlayerCookie::Input()
{
	if (m_bSlide)
		return;
	if (m_bDie)
		return;
	if (m_bDubbleJump)
		return;
	if (m_bControl == false)
		return;

	m_pPlayer->stopAllActions();
	char str[100] = { 0 };
	if (m_bJump)
	{
		m_bDubbleJump = true;

		Vector<SpriteFrame*> pAniFrames;
		
		for (int i = 1; i < 5; i++)
		{
			sprintf(str, "cookie_%02d_jump%02d.png", GameManager::GetInstance()->GetUserData()->iCharacter, i);

			SpriteFrame* Frame = m_pCharacterCache->getSpriteFrameByName(str);
			pAniFrames.pushBack(Frame);
		}
		SpriteFrame* Frame;
		sprintf(str, "cookie_%02d_jump01.png", GameManager::GetInstance()->GetUserData()->iCharacter);
		Frame = m_pCharacterCache->getSpriteFrameByName(str);
	
		pAniFrames.pushBack(Frame);

		Animation* pAnimation = Animation::createWithSpriteFrames(pAniFrames, 0.12f);
		Animate* pAnimate = Animate::create(pAnimation);
		m_pPlayer->runAction(pAnimate);

		JumpBy* pJumpBy = JumpBy::create(0.7f, Vec2(0, -100), 200, 1);
		auto pSetJump = CallFuncN::create(CC_CALLBACK_0(PlayerCookie::SetRunAnimate, this));
		Sequence* pSequence = Sequence::create(pJumpBy, pSetJump, nullptr);
		m_pPlayer->runAction(pSequence);
	}
	else
	{
		m_bJump = true;
		sprintf(str, "cookie_%02d_jump00.png", GameManager::GetInstance()->GetUserData()->iCharacter);
		m_pPlayer->setDisplayFrame(m_pCharacterCache->getSpriteFrameByName(str));


		JumpBy* pJumpBy = JumpBy::create(0.5f, Vec2(0, 0), 150, 1);
		auto pSetJump = CallFuncN::create(CC_CALLBACK_0(PlayerCookie::SetRunAnimate, this));
		Sequence* pSequence = Sequence::create(pJumpBy, pSetJump, nullptr);
		m_pPlayer->runAction(pSequence);
	}
}
void  PlayerCookie::InputFever()
{
	if (m_bFeverAction)
		return;
	if (m_bControl == false)
		return;

	char str[100] = { 0 };
	sprintf(str, "cookie_%02d_jump01.png", GameManager::GetInstance()->GetUserData()->iCharacter);
	m_pPlayer->setPosition(Vec2(m_pPlayer->getPosition().x, m_pPlayer->getPosition().y + 6));
	m_pPlayer->setDisplayFrame(m_pCharacterCache->getSpriteFrameByName(str));
	m_bPush = true;
	
	return;
}
void PlayerCookie::ShowCookie()
{
	m_pPlayer->setOpacity(255);
}
void PlayerCookie::InputPush()
{
	if (m_bSlide)
		return;
	if (m_bJump)
		return;
	if (m_bDubbleJump)
		return;
	if (m_bDie)
		return;
	if (m_bFever)
		return;
	if (m_bControl == false)
		return;

	m_bSlide = true;
	m_pPlayer->stopAllActions();

	m_pPlayer->setPosition(Vec2(m_pPlayer->getPosition().x, 95));
	Vector<SpriteFrame*> pAniFrames;
	char str[100] = { 0 };
	for (int i = 0; i < 2; i++)
	{
		sprintf(str, "cookie_%02d_slide%02d.png", GameManager::GetInstance()->GetUserData()->iCharacter, i);
		
		SpriteFrame* Frame = m_pCharacterCache->getSpriteFrameByName(str);
		pAniFrames.pushBack(Frame);
	}
	Animation* pAnimation = Animation::createWithSpriteFrames(pAniFrames, 0.1f);
	Animate* pAnimate = Animate::create(pAnimation);
	RepeatForever* pRepeatForever = RepeatForever::create(pAnimate);
	m_pPlayer->runAction(pRepeatForever);
}
void PlayerCookie::SetRunAnimate()
{
	if (m_bDie)
	{
		char str[100] = { 0 };
		m_pPlayer->stopAllActions();
		sprintf(str, "cookie_%02d_health_end.png", GameManager::GetInstance()->GetUserData()->iCharacter);
		m_pPlayer->setDisplayFrame(m_pCharacterCache->getSpriteFrameByName(str));

		if (m_pPlayer->getPosition().y > 0)
			m_pPlayer->setPosition(Vec2(m_pPlayer->getPosition().x, 95));
		return;
	}

	ResetState();
	m_bControl = true;

	Vector<SpriteFrame*> pAniFrames;
	char str[100] = { 0 };
	for (int i = 0; i < 4; i++)
	{
		sprintf(str, "cookie_%02d_run%02d.png", GameManager::GetInstance()->GetUserData()->iCharacter, i);
		
		SpriteFrame* Frame = m_pCharacterCache->getSpriteFrameByName(str);
		pAniFrames.pushBack(Frame);
	}
	Animation* pAnimation = Animation::createWithSpriteFrames(pAniFrames, 0.05f);
	Animate* pAnimate = Animate::create(pAnimation);
	RepeatForever* pRepeatForever = RepeatForever::create(pAnimate);
	m_pPlayer->runAction(pRepeatForever);
}

void PlayerCookie::ResetState()
{
	m_pPlayer->stopAllActions();
	m_bJump = false;
	m_bSlide = false;
	m_bDubbleJump = false;

	if (m_pPlayer->getScale() > 2.0f)
		m_pPlayer->setPosition(Vec2(m_pPlayer->getPosition().x, 180));
	else
		m_pPlayer->setPosition(Vec2(m_pPlayer->getPosition().x, 130));
}

void PlayerCookie::SetVisible()
{
	if (m_pPlayer->isVisible() == false)
		m_pPlayer->setVisible(true);
	else
		m_pPlayer->setVisible(false);
}

void PlayerCookie::SetHealthCookie()
{
	m_bDie = true;

	if (m_bSlide)
		return;
	if (m_bJump)
		return;

	m_pPlayer->stopAllActions();
	char str[100] = { 0 };
	sprintf(str, "cookie_%02d_health_end.png", GameManager::GetInstance()->GetUserData()->iCharacter);
	m_pPlayer->setDisplayFrame(m_pCharacterCache->getSpriteFrameByName(str));

	if (m_pPlayer->getPosition().y > 0)
		m_pPlayer->setPosition(Vec2(m_pPlayer->getPosition().x, 110));
}

void PlayerCookie::ChangeGodMode()
{
	if (m_bGod)
		m_bGod = false;
	else
		m_bGod = true;
}

void PlayerCookie::SetItem(string Item)
{
	if (Item == "play_item_big.png")
		m_bItemBig = true;
	else if(Item == "play_item_speed.png")
		m_bItemBoost = true;
}

void PlayerCookie::ResetItem(string Item)
{
	if (Item == "play_item_big.png")
		m_bItemBig = false;
	else if (Item == "play_item_speed.png")
		m_bItemBoost = false;
}
void PlayerCookie::ShowEffect()
{
	Sprite* pCoinEffect;
	if (m_pPlayer->getName() == "Coin")
		pCoinEffect = Sprite::createWithSpriteFrameName("effect_coin.png");
	else if (m_pPlayer->getName() == "Jelly")
		pCoinEffect = Sprite::createWithSpriteFrameName("effect_jelly.png");
	else
		pCoinEffect = Sprite::createWithSpriteFrameName("effect_jelly.png");
	pCoinEffect->setPosition(m_pPlayer->getPosition().x - 30, m_pPlayer->getPosition().y);
	m_vcCoinEffect.pushBack(pCoinEffect);
	m_pScene->addChild(pCoinEffect, 2);
}
void PlayerCookie::SetJump()
{
	m_bJump = true;
}
void PlayerCookie::SetDubbleJump()
{
	m_bDubbleJump = true;
}
void PlayerCookie::SetRunAni()
{
	return;
}
bool PlayerCookie::CheckCrash()
{
	if (m_bGod)
		return false;

	if (m_pObjectManager->CheckCrashCookie(m_pPlayer))
		return true;

	return false;
}

void PlayerCookie::SetFeverMode()
{
	m_pPlayer->stopAllActions();
	m_bFever = true;
	m_bFeverAction = true;

	auto pGrivaty = CallFuncN::create(CC_CALLBACK_0(PlayerCookie::ActionFever, this));
	DelayTime* pDelay = DelayTime::create(1.0f);
	MoveTo* pMoveTo = MoveTo::create(1.0f, Vec2(m_pPlayer->getPosition().x, ResManager::GetInstance()->GetDisplaySize().height + 100));
	Sequence* pSequence = Sequence::create(pMoveTo, pDelay, pGrivaty, nullptr);
	m_pPlayer->runAction(pSequence);

	Sprite* pFeverEffect = Sprite::create("fever_light.png");
	pFeverEffect->setOpacity(0);
	pFeverEffect->setPosition(Vec2(20, 0));
	m_pPlayer->addChild(pFeverEffect);
	
	FadeIn* pFadeIn = FadeIn::create(5.0f);
	RotateBy* pRotateBy = RotateBy::create(1.0f, 360);
	Repeat* pRepeat = Repeat::create(pRotateBy, 5);
	pFeverEffect->runAction(pRepeat);
	pFeverEffect->runAction(pFadeIn);
}
void PlayerCookie::UnFeverMode()
{
	m_bFever = false;
	m_bControl = false;
}

void PlayerCookie::UnPush()
{
	if (m_bFeverAction)
		return;
	m_bPush = false;
}
void PlayerCookie::ActionFever()
{
	m_bFeverAction = false;
	m_pPlayer->removeAllChildren();
}