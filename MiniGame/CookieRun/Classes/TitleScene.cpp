#include "TitleScene.h"
#include "LobbyScene.h"
#include "time.h"

#include "ResManager.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

TitleScene::TitleScene()
{
	srand(time(NULL));
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
	m_Size = Director::getInstance()->getVisibleSize();
	m_Origin = Director::getInstance()->getVisibleOrigin();
	ResManager::GetInstance()->Init(m_Size, m_Origin);
	
	UserData* stUserData = new UserData;
	stUserData->iCash = 0;
	stUserData->iCharacter = 0;
	stUserData->iMoney = 100000;
	stUserData->iJellyLevel = 1;
	stUserData->iHpLevel = 1;
	stUserData->iTimeLevel = 1;

	GameManager::GetInstance()->Init(stUserData);

	auto Mouse_Listener = EventListenerMouse::create();
	Mouse_Listener->onMouseDown = CC_CALLBACK_1(TitleScene::onMouseDown, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(Mouse_Listener, this);

	InitGame();
	return true;
}

void TitleScene::InitGame()	
{
	m_pBackGround = Sprite::create("title_bg.png");
	ResManager::GetInstance()->SetFullSizeSprite(m_pBackGround);
	this->addChild(m_pBackGround, 0);
	
	m_pTitleLogo = Sprite::create("title_logo.png");
	m_pTitleLogo->setPosition(m_Size.width / 2, m_Size.height / 3);
	this->addChild(m_pTitleLogo, 1);

	auto pScaleBy = ScaleBy::create(1.0f, 1.1);
	auto pReverse = pScaleBy->reverse();
	auto pSequence = Sequence::create(pScaleBy, pReverse, nullptr);
	auto pRepeat = RepeatForever::create(pSequence);
	m_pTitleLogo->runAction(pRepeat);


	auto Label = Label::createWithTTF("Touch To Start", "fonts/NanumGothic.ttf", 52);
	m_pTitleLogo->addChild(Label, 1);
	Label->setAnchorPoint(Vec2(0, 1.0f));
	Label->setPosition(Vec2(50, 30));
}

void TitleScene::onMouseDown(Event* event)
{
	Director::getInstance()->replaceScene(LobbyScene::create());
}