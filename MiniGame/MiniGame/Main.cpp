#include <IncludeDefine.h>
#include <EngineMain.h>

#include "CardGameScene.h"
#include "TitleScene.h"
#include "GameMenu.h"
#include "GameMenuScene.h"
#include "AirplaneScene.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	Engine::EngineMain engine("MiniGame", 540, 810);
	Engine::SceneManager::GetInstance()->GenerateScene(new TitleScene);
	Engine::SceneManager::GetInstance()->GenerateScene(new GameMenuScene);
	Engine::SceneManager::GetInstance()->GenerateScene(new CardGameScene);
	Engine::SceneManager::GetInstance()->GenerateScene(new AirplaneScene);
	return engine.StartEngine(hInstance);
}

