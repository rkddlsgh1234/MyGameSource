#include "GameMenu.h"



GameMenu::GameMenu()
{
}


GameMenu::~GameMenu()
{
}

void GameMenu::Init(int x, int y, Engine::BitMap* pBitMap, SCENE_STATE GameType, BUTTON_TYPE ButtonType)
{
	m_fX = (float)x;
	m_fY = (float)y;

	m_fButtonX = m_fX + 400;
	m_fButtonY = m_fY + 25;
	m_fButtonMotionDelay = 0;
	m_iCount = 0;
	m_eButtonState = BUTTON_STATE_WAIT;
	m_eGameType = GameType;
	m_pBitMap = pBitMap;

	if (ButtonType == BUTTON_TYPE_SELECT)
	{
		m_pButton[0] = Engine::ResManager::GetInstance()->GetBitMap("Button_Select_00.bmp");
		m_pButton[1] = Engine::ResManager::GetInstance()->GetBitMap("Button_Select_01.bmp");
	}
	else if (ButtonType == BUTTON_TYPE_OK)
	{
		m_pButton[0] = Engine::ResManager::GetInstance()->GetBitMap("Button_Ok_00.bmp");
		m_pButton[1] = Engine::ResManager::GetInstance()->GetBitMap("Button_Ok_01.bmp");
	}
}

void GameMenu::Draw()
{
	m_pBitMap->Draw(m_fX, m_fY);

	if(m_iCount == 1)
		m_pButton[1]->Draw(m_fButtonX, m_fButtonY);
	else
		m_pButton[0]->Draw(m_fButtonX, m_fButtonY);
}

void GameMenu::Motion(float fElapseTime)
{
	if (m_eButtonState != BUTTON_STATE_PUSH)
		return;
	if (m_iCount == BUTTON_END)
		return;

	m_fButtonMotionDelay = m_fButtonMotionDelay + 100 * fElapseTime;

	if (m_fButtonMotionDelay > 3)
	{
		m_fButtonMotionDelay = 0;
		m_iCount++;
	}		

	
}

void GameMenu::ChangeButtonState()
{
	m_eButtonState = BUTTON_STATE_PUSH;
}

void GameMenu::DestoryButton()
{
	m_eButtonState = BUTTON_STATE_DESTORY;
}

void GameMenu::SetButtonPos(float x, float y)
{
	if(x != NULL)
		m_fButtonX = x;
	if (y != NULL)
		m_fButtonY = y;
}