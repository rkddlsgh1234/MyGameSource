#include "GameMenuScene.h"

GameMenuScene::GameMenuScene()
{
}


GameMenuScene::~GameMenuScene()
{
}

void GameMenuScene::Init()
{
	m_pBackGround = Engine::ResManager::GetInstance()->GetBitMap("TitleDark.bmp");
	m_pSelectBoard = Engine::ResManager::GetInstance()->GetBitMap("SelectGameBoard.bmp");
	m_fChangeSceneDelay = 0;


	for (int i = 0; i < 2; i++)
	{
		GameMenu* pGameMenu = new GameMenu;
		if(i == 0)
			pGameMenu->Init(35, 215 + (i * 90), Engine::ResManager::GetInstance()->GetBitMap("SelectCardGameBoard.bmp"), SCENE_CARD_GAME, BUTTON_TYPE_SELECT);
		else
			pGameMenu->Init(35, 215 + (i * 90), Engine::ResManager::GetInstance()->GetBitMap("SelecrAirplaneGameBoard.bmp"), SCENE_AIRPLANE_GAME, BUTTON_TYPE_SELECT);
		m_vcSelectGameBoard.push_back(pGameMenu);
	}
}

void GameMenuScene::Draw()
{
	m_pBackGround->DrawBitblt(0, 0);
	m_pSelectBoard->Draw(10, 100);

	for (auto i = m_vcSelectGameBoard.begin(); i != m_vcSelectGameBoard.end(); i++)
	{
		(*i)->Draw();
	}
	
}

void GameMenuScene::Updata(float fElaseTime)
{
	for (auto i = m_vcSelectGameBoard.begin(); i != m_vcSelectGameBoard.end(); i++)
	{
		(*i)->Motion(fElaseTime);

		if ((*i)->GetButtonState() == BUTTON_STATE_PUSH)
		{
			m_fChangeSceneDelay = m_fChangeSceneDelay + 70 * fElaseTime;

			if(m_fChangeSceneDelay > 30)
				Engine::SceneManager::GetInstance()->ChangeScene((*i)->GetGameType());
			
			return;
		}
	}
}

bool GameMenuScene::Input(float fElaseTime)
{
	if (Engine::InputManager::GetInstance()->GetMousePos().x == NULL)
		return false;

	for (auto i = m_vcSelectGameBoard.begin(); i != m_vcSelectGameBoard.end(); i++)
	{
		if ((*i)->GetButtonState() == BUTTON_STATE_PUSH)
			return false;

		POINT pt = { Engine::InputManager::GetInstance()->GetMousePos().x, Engine::InputManager::GetInstance()->GetMousePos().y };
		RECT rc = (*i)->GetButtonSize();

		if (PtInRect(&rc, pt))
		{
			(*i)->ChangeButtonState();
		}
	}

	return false;
}

void GameMenuScene::Release()
{
	for (auto i = m_vcSelectGameBoard.begin(); i != m_vcSelectGameBoard.end(); i++)
	{
		SAFE_DELETE((*i));
	}
	m_vcSelectGameBoard.clear();
}
