#pragma once
#include <IncludeDefine.h>
#include <Scene.h>
#include <BitMap.h>
#include <SceneManager.h>
#include <InputManager.h>

#include "GameMenu.h"
#include "CardGameScene.h"

class TitleScene : public Engine::Scene
{
private:
	Engine::BitMap* m_pBackGround;
public:
	TitleScene();
	virtual ~TitleScene();

	virtual void Init();
	virtual bool Input(float fElapseTime);
	virtual void Draw();
	virtual void Release();
};

