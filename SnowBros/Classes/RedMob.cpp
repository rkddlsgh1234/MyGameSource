#include "RedMob.h"
#include "ResManager.h"

USING_NS_CC;

RedMob::RedMob()
{
}

RedMob::~RedMob()
{
}

void RedMob::Init(cocos2d::Scene* pScene, cocos2d::SpriteFrameCache* pIceCache, cocos2d::SpriteFrameCache* pMobCache, int iMonsterNum)
{
	Enemy::Init(pScene, pIceCache, pMobCache, iMonsterNum);
}
void RedMob::CountHit(cocos2d::EventKeyboard::KeyCode keyCode)
{
	Enemy::CountHit(keyCode);
}

void RedMob::MoveEnemy()
{
	Enemy::MoveEnemy();
}
void RedMob::MoveIce(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Sprite* pPlayer)
{
	RedMob::MoveIce(keyCode, pPlayer);
}
void RedMob::InitEnemy(cocos2d::Vec2 VecPos, cocos2d::EventKeyboard::KeyCode keyCode)
{
	RedMob::InitEnemy(VecPos, keyCode);
}
