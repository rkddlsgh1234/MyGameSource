#include "Playerble.h"
#include "ResManager.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

USING_NS_CC;

Playerble::Playerble()
{
}


Playerble::~Playerble()
{
}


void Playerble::Init(cocos2d::Scene* pScene)
{
	m_pChCache = SpriteFrameCache::getInstance();
	m_pChCache->addSpriteFramesWithFile("ch_00.plist");

	m_pPlayer = Sprite::createWithSpriteFrameName("ch_left_wait.png");
	m_pPlayer->setPosition(Vec2(320, 85));
	pScene->addChild(m_pPlayer, 10);

	m_pPlayer->setVisible(false);
	m_bLife = false;

	m_PlayerWay = EventKeyboard::KeyCode::KEY_LEFT_ARROW;
	ResetState();
	m_pScene = pScene;
	m_iSpeed = 3;
}
void Playerble::ResetState()
{
	m_bAnimation = false;
	m_bGod = false;
	m_bFall = false;
	m_bJump = false;
	m_bAttack = false;
	m_bRun = false;
	m_bPushIce = false;
	m_bIceRun = false;
}

void Playerble::Start()
{
	m_pPlayer->setVisible(true);
	m_bLife = true;
	m_bJump = false;
}

void Playerble::MovePlayer(cocos2d::EventKeyboard::KeyCode keyCode)
{
	if (m_pPlayer->getBoundingBox().getMinX() - 3 <= 30 && keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
		return;
	else if (m_pPlayer->getBoundingBox().getMaxX() + 3 >= ResManager::GetInstance()->GetDisplaySize().width - 20 && keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
		return;

	switch (keyCode)
	{
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		{
			m_bRun = true;
			m_pPlayer->setPosition(Vec2(m_pPlayer->getPosition().x - m_iSpeed, m_pPlayer->getPosition().y));
			SetAnimation(keyCode);
			SetFallAnimation();
			break;
		}
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		{
			m_bRun = true;
			m_pPlayer->setPosition(Vec2(m_pPlayer->getPosition().x + m_iSpeed, m_pPlayer->getPosition().y));
			SetAnimation(keyCode);
			SetFallAnimation();

			break;
		}
	}
}

void Playerble::Input(cocos2d::EventKeyboard::KeyCode keyCode)
{
	switch (keyCode)
	{
		case EventKeyboard::KeyCode::KEY_SPACE:
		{
			if (m_bJump)
				return;
			if (m_bAttack)
				break;
			if (m_bFall)
				break;
			if (m_bPushIce)
				break;
			m_bJump = true;
			m_bAnimation = false;
			m_pPlayer->stopActionByTag(0);
			auto pJumpBy = JumpBy::create(0.5f, Vec2(0, 100), 42 , 1);
			auto pResetJump = CallFuncN::create(CC_CALLBACK_0(Playerble::ResetJump, this));
			auto pSeqnence = Sequence::create(pJumpBy, pResetJump, nullptr);
			m_pPlayer->runAction(pSeqnence);
			SetAnimation(keyCode);
			SimpleAudioEngine::getInstance()->playEffect("sound/fx_jump_00_01.wav");
			break;
		}
		case  EventKeyboard::KeyCode::KEY_A:
		{
			if (m_bJump)
				return;
			if (m_bAttack)
				break;
			if (m_bFall)
				break;
			if (m_bRun)
				break;
			m_bAttack = true;
			MoveBy* pMoveBy;
			if (m_PlayerWay == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
			{
				pMoveBy = MoveBy::create(0.2f, Vec2(-100, 0));
				m_pBullet = Sprite::createWithSpriteFrameName("left_attack.png");
			}
			else if (m_PlayerWay == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
			{
				pMoveBy = MoveBy::create(0.2f, Vec2(100, 0));
				m_pBullet = Sprite::createWithSpriteFrameName("right_attack.png");
			}
			m_pBullet->setPosition(m_pPlayer->getPosition().x, m_pPlayer->getBoundingBox().getMidY());
			m_pScene->addChild(m_pBullet, 1);

			auto pHide = Hide::create();
			auto pSequence = Sequence::create(pMoveBy, pHide, nullptr);
			m_pBullet->runAction(pSequence);
			SetAnimation(keyCode);
			SimpleAudioEngine::getInstance()->playEffect("sound/fx_laser_00_01.wav");
			break;
		}
	}
	
}
void Playerble::SetAnimation(cocos2d::EventKeyboard::KeyCode keyCode)
{
	if (m_bAnimation)
		return;
	if (m_bFall)
		return;
	if (m_bIceRun)
		return;

	m_bAnimation = true;

	char str[100] = { 0 };
	switch (keyCode)
	{
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		{
			m_PlayerWay = keyCode;

			Vector<SpriteFrame*> pAniFrames;

			if (m_bPushIce)
			{
				for (int i = 0; i < 4; i++)
				{
					sprintf(str, "ch_left_push_%02d.png", i);
					SpriteFrame* Frame = m_pChCache->getSpriteFrameByName(str);
					pAniFrames.pushBack(Frame);
				}
			}
			else
			{
				if (m_iSpeed != 5)
				{
					for (int i = 0; i < 4; i++)
					{
						sprintf(str, "ch_left_run_%02d.png", i);
						SpriteFrame* Frame = m_pChCache->getSpriteFrameByName(str);
						pAniFrames.pushBack(Frame);
					}
				}
				else
				{
					for (int i = 0; i < 4; i++)
					{
						sprintf(str, "ch_left_rush_%02d.png", i);
						SpriteFrame* Frame = m_pChCache->getSpriteFrameByName(str);
						pAniFrames.pushBack(Frame);
					}
				}
			}

			Animation* pAnimation = Animation::createWithSpriteFrames(pAniFrames, 0.1f);
			Animate* pAnimate = Animate::create(pAnimation);
			auto pRepeat = RepeatForever::create(pAnimate);
			m_pPlayer->runAction(pRepeat);
			pRepeat->setTag(0);
			break;
		}
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		{
			m_PlayerWay = keyCode;

			Vector<SpriteFrame*> pAniFrames;

			if (m_bPushIce)
			{
				for (int i = 0; i < 4; i++)
				{
					sprintf(str, "ch_right_push_%02d.png", i);
					SpriteFrame* Frame = m_pChCache->getSpriteFrameByName(str);
					pAniFrames.pushBack(Frame);
				}
			}
			else
			{
				if (m_iSpeed != 5)
				{
					for (int i = 0; i < 4; i++)
					{
						sprintf(str, "ch_right_run_%02d.png", i);
						SpriteFrame* Frame = m_pChCache->getSpriteFrameByName(str);
						pAniFrames.pushBack(Frame);
					}
				}
				else
				{
					for (int i = 0; i < 4; i++)
					{
						sprintf(str, "ch_right_rush_%02d.png", i);
						SpriteFrame* Frame = m_pChCache->getSpriteFrameByName(str);
						pAniFrames.pushBack(Frame);
					}
				}
			}

			Animation* pAnimation = Animation::createWithSpriteFrames(pAniFrames, 0.1f);
			Animate* pAnimate = Animate::create(pAnimation);
			auto pRepeat = RepeatForever::create(pAnimate);
			pRepeat->setTag(0);
			m_pPlayer->runAction(pRepeat);

			break;
		}
		case EventKeyboard::KeyCode::KEY_SPACE:
		{
			if (m_bPushIce)
				break;
			Vector<SpriteFrame*> pAniFrames;
			if (m_PlayerWay == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
			{
				for (int i = 0; i < 5; i++)
				{
					sprintf(str, "ch_left_jump_%02d.png", i);
					SpriteFrame* Frame = m_pChCache->getSpriteFrameByName(str);
					pAniFrames.pushBack(Frame);
				}		
				SpriteFrame* Frame = m_pChCache->getSpriteFrameByName("ch_left_wait.png");
				pAniFrames.pushBack(Frame);
			}
			else if (m_PlayerWay == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
			{
				for (int i = 0; i < 5; i++)
				{
					sprintf(str, "ch_right_jump_%02d.png", i);
					SpriteFrame* Frame = m_pChCache->getSpriteFrameByName(str);
					pAniFrames.pushBack(Frame);
				}
				SpriteFrame* Frame = m_pChCache->getSpriteFrameByName("ch_right_wait.png");
				pAniFrames.pushBack(Frame);
			}
		
			Animation* pAnimation = Animation::createWithSpriteFrames(pAniFrames, 0.1f);
			Animate* pAnimate = Animate::create(pAnimation);
			pAnimate->setTag(0);
			m_pPlayer->runAction(pAnimate);
			break;
		}
		case EventKeyboard::KeyCode::KEY_A:
		{
			Vector<SpriteFrame*> pAniFrames;
			if (m_PlayerWay == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
			{
				for (int i = 0; i < 3; i++)
				{
					sprintf(str, "ch_left_attack_%02d.png", i);
					SpriteFrame* Frame = m_pChCache->getSpriteFrameByName(str);
					pAniFrames.pushBack(Frame);
				}
				SpriteFrame* Frame = m_pChCache->getSpriteFrameByName("ch_left_wait.png");
				pAniFrames.pushBack(Frame);
			}
			else if (m_PlayerWay == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
			{
				for (int i = 0; i < 3; i++)
				{
					sprintf(str, "ch_right_attack_%02d.png", i);
					SpriteFrame* Frame = m_pChCache->getSpriteFrameByName(str);
					pAniFrames.pushBack(Frame);
				}
				SpriteFrame* Frame = m_pChCache->getSpriteFrameByName("ch_right_wait.png");
				pAniFrames.pushBack(Frame);
			}
			auto pResetAttack = CallFuncN::create(CC_CALLBACK_0(Playerble::ResetAttack, this));
			Animation* pAnimation = Animation::createWithSpriteFrames(pAniFrames, 0.07f);
			Animate* pAnimate = Animate::create(pAnimation);
			pAnimate->setTag(0);
			auto pSequence = Sequence::create(pAnimate, pResetAttack, nullptr);
			m_pPlayer->runAction(pSequence);
			break;
		}
	}
}
void Playerble::ReleaseInput()
{
	if (m_bFall)
		return;
	if (m_bJump)
		return;
	if (m_bAttack)
		return;
	if (m_bRun)
	{
		m_pPlayer->stopActionByTag(0);
		m_bRun = false;
	}
	m_bAnimation = false;

	if(m_PlayerWay == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
		m_pPlayer->setDisplayFrame(m_pChCache->getSpriteFrameByName("ch_right_wait.png"));
	else if(m_PlayerWay == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
		m_pPlayer->setDisplayFrame(m_pChCache->getSpriteFrameByName("ch_left_wait.png"));
}

void Playerble::SetFallAnimation()
{
	if (m_bFall == false)
		return;
	if (m_bJump)
		return;
	if (m_bAttack)
		return;

	if (m_PlayerWay == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
		m_pPlayer->setDisplayFrame(m_pChCache->getSpriteFrameByName("ch_right_jump_down.png"));
	else if (m_PlayerWay == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
		m_pPlayer->setDisplayFrame(m_pChCache->getSpriteFrameByName("ch_left_jump_down.png"));
}
void Playerble::ResetAnimation()
{
	m_bJump = false;
	m_pPlayer->stopActionByTag(0);

	if (m_PlayerWay == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
		m_pPlayer->setDisplayFrame(m_pChCache->getSpriteFrameByName("ch_right_wait.png"));
	else if (m_PlayerWay == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
		m_pPlayer->setDisplayFrame(m_pChCache->getSpriteFrameByName("ch_left_wait.png"));
}
void Playerble::SetFall()
{
	if (m_bFall)
		return;
	if (m_bJump)
		return;

	m_bFall = true;
	m_pPlayer->stopActionByTag(0);
	SetFallAnimation();
}
void Playerble::ResetFall()
{
	if (m_bFall == false)
		return;
	if (m_bJump)
		return;

	m_bFall = false;
	m_bAnimation = false;

	if (m_PlayerWay == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
		m_pPlayer->setDisplayFrame(m_pChCache->getSpriteFrameByName("ch_right_wait.png"));
	else if (m_PlayerWay == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
		m_pPlayer->setDisplayFrame(m_pChCache->getSpriteFrameByName("ch_left_wait.png"));
}
void Playerble::ResetJump()
{
	m_bJump = false;
	m_bAnimation = false;
	if(m_iSpeed == 5)
		SetAnimation(m_PlayerWay);
}
void Playerble::ResetMotion()
{
	m_bAnimation = false;
}
void Playerble::ResetAttack()
{
	m_bAttack = false;
	m_bAnimation = false;
}

void Playerble::SetPushIce()
{
	if (m_bPushIce == false)
	{
		m_bPushIce = true;
		m_bAnimation = false;
		m_pPlayer->stopAllActions();
		SetAnimation(m_PlayerWay);
	}
}
void Playerble::ResetPushIce()
{
	if (m_bPushIce)
	{
		m_bPushIce = false;
		m_bAnimation = false;
		m_pPlayer->stopAllActions();
		SetAnimation(m_PlayerWay);
	}
}

void Playerble::SetIceRunAnimation()
{
	if (m_bIceRun)
		return;

	m_bIceRun = true;
	m_pPlayer->stopAllActions();

	Vector<SpriteFrame*> pAniFrames;
	char str[100] = { 0 };
	for (int i = 0; i < 8; i++)
	{
		sprintf(str, "ch_ice_run_%02d.png", i);
		SpriteFrame* Frame = m_pChCache->getSpriteFrameByName(str);
		pAniFrames.pushBack(Frame);
	}

	Animation* pAnimation = Animation::createWithSpriteFrames(pAniFrames, 0.05f);
	Animate* pAnimate = Animate::create(pAnimation);
	auto pRepeat = RepeatForever::create(pAnimate);
	m_pPlayer->runAction(pRepeat);
}

void Playerble::ResetIceRun()
{
	if (m_bIceRun == false)
		return;

	if (m_PlayerWay == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
		m_pPlayer->setDisplayFrame(m_pChCache->getSpriteFrameByName("ch_right_wait.png"));
	else if (m_PlayerWay == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
		m_pPlayer->setDisplayFrame(m_pChCache->getSpriteFrameByName("ch_left_wait.png"));
	m_bIceRun = false;
	m_bAnimation = false;
	m_pPlayer->stopAllActions();
}

void Playerble::SetDead()
{
	if (m_bLife == false)
		return;

	m_pPlayer->stopAllActions();
	m_bLife = false;
	m_bGod = true;
	m_iSpeed = 3;

	Vector<SpriteFrame*> pAniFrames;
	char str[100] = { 0 };

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			sprintf(str, "ch_dead_%02d.png", j);
			SpriteFrame* Frame = m_pChCache->getSpriteFrameByName(str);
			pAniFrames.pushBack(Frame);
		}
	}
	for (int i = 2; i < 7; i++)
	{
		sprintf(str, "ch_dead_%02d.png", i);
		SpriteFrame* Frame = m_pChCache->getSpriteFrameByName(str);
		pAniFrames.pushBack(Frame);
	}

	Animation* pAnimation = Animation::createWithSpriteFrames(pAniFrames, 0.1f);
	Animate* pAnimate = Animate::create(pAnimation);
	auto pHide = Hide::create();
	auto pSequence = Sequence::create(pAnimate, pHide, nullptr);
	m_pPlayer->runAction(pSequence);
	SimpleAudioEngine::getInstance()->playEffect("sound/fx_snow_hit_01.wav");
}

void Playerble::SetLife()
{
	m_bLife = true;
	if (m_PlayerWay == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
		m_pPlayer->setDisplayFrame(m_pChCache->getSpriteFrameByName("ch_right_wait.png"));
	else if (m_PlayerWay == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
		m_pPlayer->setDisplayFrame(m_pChCache->getSpriteFrameByName("ch_left_wait.png"));
}

void Playerble::SetGodMode()
{
	m_bGod = false;
}
void Playerble::ChangeGodMode()
{
	m_bGod = true;
}

void Playerble::SetSpeed()
{
	m_iSpeed = 5;
}

void Playerble::ChangeTexture()
{
	m_pPlayer->stopAllActions();
	if (m_PlayerWay == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
		m_pPlayer->setDisplayFrame(m_pChCache->getSpriteFrameByName("ch_right_jump_down.png"));
	else if (m_PlayerWay == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
		m_pPlayer->setDisplayFrame(m_pChCache->getSpriteFrameByName("ch_left_jump_down.png"));
}

void Playerble::RespawnNextRound()
{
	m_pPlayer->setDisplayFrame(m_pChCache->getSpriteFrameByName("ch_left_wait.png"));
	m_pPlayer->setPosition(Vec2(320, 85));

	Sprite* pEffect = Sprite::createWithSpriteFrameName("effect_respawn_00.png");
	pEffect->setAnchorPoint(Vec2(0.5f, 0));
	pEffect->setPosition(m_pPlayer->getPosition().x, m_pPlayer->getPosition().y - 40);

	Vector<SpriteFrame*> pAniFrames;
	char str[100] = { 0 };
	for (int i = 0; i < 9; i++)
	{
		sprintf(str, "effect_respawn_%02d.png", i);
		SpriteFrame* Frame = m_pChCache->getSpriteFrameByName(str);
		pAniFrames.pushBack(Frame);
	}

	Animation* pAnimation = Animation::createWithSpriteFrames(pAniFrames, 0.07f);
	Animate* pAnimate = Animate::create(pAnimation);
	auto pRespawn = CallFuncN::create(CC_CALLBACK_0(Playerble::RespawnRound, this));
	auto pHide = Hide::create();
	auto pSequnece = Sequence::create(pAnimate, pHide, pRespawn, nullptr);
	pEffect->runAction(pSequnece);
	m_pScene->addChild(pEffect, 6);
}
void Playerble::RespawnRound()
{
	m_pPlayer->setVisible(true);
	ResetState();
}