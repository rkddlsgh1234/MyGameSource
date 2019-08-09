#pragma once

#include "cocos2d.h"

class ResManager
{
private:
	static ResManager* m_Instance;

	cocos2d::Size m_Size;
	cocos2d::Vec2 m_Origin;

	ResManager();
	~ResManager();
public:
	static ResManager* GetInstance();

	void Init(cocos2d::Size Size, cocos2d::Vec2 Origin);
	void SetFullSizeSprite(cocos2d::Sprite* pSprite);

	virtual inline cocos2d::Size GetDisplaySize()
	{
		return m_Size;
	}
};
