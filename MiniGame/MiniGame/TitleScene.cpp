#include "TitleScene.h"


TitleScene::TitleScene()
{
}


TitleScene::~TitleScene()
{
}

void TitleScene::Init()
{
	m_pBackGround = Engine::ResManager::GetInstance()->GetBitMap("Title.bmp");
}

bool TitleScene::Input(float fElaseTime)
{
	if (Engine::InputManager::GetInstance()->GetMousePushState() == false)
		return false;

	Engine::SceneManager::GetInstance()->ChangeScene(SCENE_GAMEMENU);

	return false;
}

void TitleScene::Draw()
{
	m_pBackGround->DrawBitblt(0, 0);
}
void TitleScene::Release()
{
}