#pragma once
#include "cocos2d.h"

#define __TITLESCENE_SCENE_H__

using namespace std;

class TitleScene : public cocos2d::Scene
{
private:
	cocos2d::Size m_Size;
	cocos2d::Vec2 m_Origin;
private:
	cocos2d::Sprite* m_pBackGround;
	cocos2d::Sprite* m_pTitleLogo;
public:
	TitleScene();
	~TitleScene();

	static cocos2d::Scene* createScene();
	virtual bool init();

	void InitGame();
	void onMouseDown(cocos2d::Event* event);

	CREATE_FUNC(TitleScene);
};
