#pragma once
#include <IncludeDefine.h>
#include <BitMap.h>
#include <SceneManager.h>
#include <Scene.h>

#include "GameMenu.h"

class GameMenuScene : public Engine::Scene
{
private:
	Engine::BitMap* m_pBackGround;
	Engine::BitMap* m_pSelectBoard;

	float m_fChangeSceneDelay;

	vector <GameMenu*> m_vcSelectGameBoard;
public:
	GameMenuScene();
	virtual ~GameMenuScene();

	virtual void Init();
	virtual void Draw();
	virtual bool Input(float fElapseTime);
	virtual void Updata(float fElaseTime);
	virtual void Release();
};

