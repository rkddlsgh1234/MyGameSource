#include "ResManager.h"

USING_NS_CC;

ResManager::ResManager()
{
}


ResManager::~ResManager()
{
	delete m_Instance;
}
ResManager* ResManager::m_Instance = NULL;

ResManager* ResManager::GetInstance()
{
	if (m_Instance == NULL)
	{
		m_Instance = new ResManager;
	}

	return m_Instance;
}

void ResManager::Init(cocos2d::Size Size, cocos2d::Vec2 Origin)
{
	m_Size = Size;
	m_Origin = Origin;
}

void ResManager::SetFullSizeSprite(cocos2d::Sprite* pSprite)
{
	pSprite->setScale(m_Size.width / pSprite->getContentSize().width, m_Size.height / pSprite->getContentSize().height);
	pSprite->setPosition(Vec2(0, 0));
	pSprite->setAnchorPoint(Vec2(0, 0));
}