#include "TitleScene.h"
#include "GameScene.h"
#include <time.h>

#include "SimpleAudioEngine.h"
#include "ResManager.h"

USING_NS_CC;

TitleScene::TitleScene()
{
	srand((unsigned int)time(NULL));
}


TitleScene::~TitleScene()
{
	
}

Scene* TitleScene::createScene()
{
	auto scene = Scene::create();
	auto layer = TitleScene::create();
	scene->addChild(layer);

	return scene;
}

bool TitleScene::init()
{
	auto m_Size = Director::getInstance()->getVisibleSize();
	auto m_Origin = Director::getInstance()->getVisibleOrigin();
	ResManager::GetInstance()->Init(m_Size, m_Origin);

	auto KeyBoard_Listener = EventListenerKeyboard::create();
	KeyBoard_Listener->onKeyPressed = CC_CALLBACK_2(TitleScene::PressKeyboard, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(KeyBoard_Listener, this);

	InitSprite();
	return true;
}

void TitleScene::InitSprite()
{
	m_pTitleLogo = Sprite::create("main_title.png");
	m_pTitleLogo->setPosition(Vec2(ResManager::GetInstance()->GetDisplaySize().width / 2, ResManager::GetInstance()->GetDisplaySize().height - 170));
	this->addChild(m_pTitleLogo);

	auto pPressText = Label::createWithTTF("PRESS ENTER", "fonts/Maplestory Bold.ttf", 35);
	pPressText->setPosition(Vec2(ResManager::GetInstance()->GetDisplaySize().width / 2, 70));
	this->addChild(pPressText);

	auto pBlink = Blink::create(0.4f, 1);
	auto pRepeatForever = RepeatForever::create(pBlink);
	pPressText->runAction(pRepeatForever);
}

void TitleScene::PressKeyboard(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_ENTER:
		Director::getInstance()->replaceScene(GameScene::create());
		break;
	}

}