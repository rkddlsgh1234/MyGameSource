#include "CardGameScene.h"
#include "GameUI.h"
#include "GameMenu.h"

CardGameScene::CardGameScene()
{
}


CardGameScene::~CardGameScene()
{
}

void CardGameScene::Init()
{	
	Engine::ResManager::GetInstance()->GetBitMap("CardBlue.bmp");
	Engine::ResManager::GetInstance()->GetBitMap("CardRed.bmp");
	Engine::ResManager::GetInstance()->GetBitMap("CardYellow.bmp");
	Engine::ResManager::GetInstance()->GetBitMap("CardGreen.bmp");

	Engine::ResManager::GetInstance()->GetBitMap("Effect_00.bmp");
	Engine::ResManager::GetInstance()->GetBitMap("Effect_01.bmp");
	Engine::ResManager::GetInstance()->GetBitMap("Effect_02.bmp");
	Engine::ResManager::GetInstance()->GetBitMap("Effect_03.bmp");
	Engine::ResManager::GetInstance()->GetBitMap("Effect_04.bmp");
	Engine::ResManager::GetInstance()->GetBitMap("Effect_05.bmp");

	m_pState[0] = Engine::ResManager::GetInstance()->GetBitMap("State_00.bmp");
	m_pState[1] = Engine::ResManager::GetInstance()->GetBitMap("State_01.bmp");
	m_pBack[0] = Engine::ResManager::GetInstance()->GetBitMap("CardBack.bmp");
	m_pBack[1] = Engine::ResManager::GetInstance()->GetBitMap("CardBack_Dark.bmp");
	TCHAR str[128];
	for (int i = 0; i < MAX_COMBO; i++)
	{
		wsprintf(str, TEXT("ComboStar_%d.bmp"), i);
		Engine::ResManager::GetInstance()->GetBitMap(str);
	}
	
	m_pGameUI = new GameUI;
	m_pGameUI->Init(DELAY_INDEX_CARD);
	m_pInfoMenu = new GameMenu;
	m_pInfoMenu->Init(INFO_MENU_POS_X, INFO_MENU_POS_Y, Engine::ResManager::GetInstance()->GetBitMap("CardGameInfo.bmp"), SCENE_END, BUTTON_TYPE_OK);
	m_pInfoMenu->SetButtonPos(150, 600);

	m_fDelay = 0;
	m_fFeverDelay = 0;

	m_iScore = 0;
	m_iCount = 0;
	m_iCombo = 0;
	m_iFieldCard = 0;
	m_iBonusCombo = 0;
	m_bCheckCombo = false;
	m_bCheckInfo = false;

	m_eGameState = GAME_STATE_WAIT;
	GenerateCard();
	GenerateCard();
}

void CardGameScene::Draw()
{
	if (m_bCheckInfo)
		m_pBack[0]->DrawBitblt(0, 0);
	else
		m_pBack[1]->DrawBitblt(0, 0);

	SetCardPostion();

	for (auto i = m_vcCard.begin(); i != m_vcCard.end(); i++)
	{
		if ((*i)->GetStateText())
		{
			if ((*i)->GetCheckCombo())
				m_pState[1]->Draw((float)(*i)->ReturnPos().x, (float)(*i)->ReturnPos().y);
			else
				m_pState[0]->Draw((float)(*i)->ReturnPos().x, (float)(*i)->ReturnPos().y);
		}
	}

	m_pGameUI->DrawFever();
	m_pGameUI->Draw();
	m_pGameUI->DrawScore(m_iScore);

	if (m_eGameState == GAME_STATE_WAIT)
	{
		if (m_bCheckInfo == false)
			m_pInfoMenu->Draw();
		else
			m_pGameUI->DrawWait(m_iCount);
	}
	if (m_eGameState == GAME_STATE_TIMEOVER)
		m_pGameUI->DrawTimeOver();
}

void CardGameScene::SetCardPostion()
{
	Card* Front = NULL;
	Card* Back = NULL;
	for (auto i = m_vcCard.begin(); i != m_vcCard.end(); i++)
	{
		if ((*i)->GetCheckScore())
			(*i)->Draw();

		(*i)->DrawEffect();

		if ((*i)->GetCardState() == CARD_STATE_FRONT)
			Front = (*i);
		else if ((*i)->GetCardState() == CARD_STATE_BACK)
		{
			Back = (*i);
			break;
		}
	}
	if (Back != NULL)
		Back->Draw();
	if (Front != NULL)
		Front->Draw();
}

bool CardGameScene::Input(float fElapseTime)
{
	if (Engine::InputManager::GetInstance()->GetMousePos().x == NULL)
		return false;

	InptInfoMenu();

	if (m_eGameState != GAME_STATE_RUN)
		return false;

	if (m_bCheckInfo == false)
		return false;

	m_ptMousept = Engine::InputManager::GetInstance()->GetMousePos();
	CheckCrash();

	return false;
}

void CardGameScene::Updata(float fElapseTime)
{
	if (UI(fElapseTime))
		return;

	ShowInfoMenu(fElapseTime);

	if (m_eGameState != GAME_STATE_RUN)
		return;
	
	GenerateCard();
	CheckCombo();
	SettingAuto(fElapseTime);
	for (auto i = m_vcCard.begin(); i != m_vcCard.end(); i++)
	{
		(*i)->Updata(fElapseTime);
	}
	
	m_ptMousept.x = NULL;
	m_ptMousept.y = NULL;
}
bool CardGameScene::UI(float fElapseTime)
{
	if (m_pGameUI->CheckButton())
	{
		Engine::SceneManager::GetInstance()->ChangeScene(SCENE_GAMEMENU);
		return true;
	}

	if (m_bCheckInfo == false)
		return false;

	if (m_eGameState == GAME_STATE_WAIT)
	{
		m_fDelay = m_fDelay + 70 * fElapseTime;
		if (m_fDelay > 100)
		{
			m_fDelay = 0;
			m_iCount++;
		}

		if (m_iCount == 2)
			m_eGameState = GAME_STATE_RUN;
	}

	if (m_eGameState == GAME_STATE_RUN)
		if (m_pGameUI->Updata(fElapseTime))
			m_eGameState = GAME_STATE_TIMEOVER;

	if (m_eGameState == GAME_STATE_TIMEOVER)
		if (m_pGameUI->TimeOverAnimation(fElapseTime))
		{
			Engine::SceneManager::GetInstance()->ChangeScene(SCENE_GAMEMENU);
			return true;
		}

	return false;
}

void CardGameScene::InptInfoMenu()
{
	POINT pt = { Engine::InputManager::GetInstance()->GetMousePos().x, Engine::InputManager::GetInstance()->GetMousePos().y };
	RECT rc = m_pInfoMenu->GetButtonSize();
	if (PtInRect(&rc, pt))
		m_pInfoMenu->ChangeButtonState();
}
void CardGameScene::ShowInfoMenu(float fElapseTime)
{
	if (m_pInfoMenu->GetButtonState() == BUTTON_STATE_PUSH)
	{
		m_pInfoMenu->Motion(fElapseTime);
		m_fDelay = m_fDelay + 70 * fElapseTime;
		if (m_fDelay > 30)
		{
			m_fDelay = 0;
			m_bCheckInfo = true;
			m_pInfoMenu->DestoryButton();
		}
	}
}

void CardGameScene::SettingAuto(float fElapseTime)
{
	if (m_fFeverDelay > 0)
	{
		m_fFeverDelay = m_fFeverDelay - 100 * fElapseTime;
		m_pGameUI->SetSize(fElapseTime);
	}
}

void CardGameScene::GenerateCard()
{
	if (m_iFieldCard == 2)
		return;

	int iRandomCard = rand() % 4;

	Engine::BitMap* pBitMap;
	if (iRandomCard == CARD_RED)
		pBitMap = Engine::ResManager::GetInstance()->GetBitMap("CardRed.bmp");
	else if (iRandomCard == CARD_BLUE)
		pBitMap = Engine::ResManager::GetInstance()->GetBitMap("CardBlue.bmp");
	else if (iRandomCard == CARD_GREEN)
		pBitMap = Engine::ResManager::GetInstance()->GetBitMap("CardGreen.bmp");
	else if (iRandomCard == CARD_YELLOW)
		pBitMap = Engine::ResManager::GetInstance()->GetBitMap("CardYellow.bmp");

	Card* pCard = new Card;
	
	pCard->Init(CARD_POS_X + ((float)m_iFieldCard * 5), CARD_POS_Y + ((float)m_iFieldCard * 5), pBitMap, iRandomCard, m_iFieldCard);

	m_vcCard.push_back(pCard);
	m_iFieldCard++;
}

void CardGameScene::Release()
{
	for (auto i = m_vcCard.begin(); i != m_vcCard.end(); i++)
	{
		SAFE_DELETE((*i));
	}
	m_vcCard.clear();
	m_pGameUI->Release();
	SAFE_DELETE(m_pGameUI);
	SAFE_DELETE(m_pInfoMenu);
}

void CardGameScene::CheckCrash()
{
	for (auto i = m_vcCard.begin(); i != m_vcCard.end(); i++)
	{
		if (m_ptMousept.x != NULL || m_ptMousept.y != NULL)
		{
			if ((*i)->CheckLife())
			{
				if ((*i)->CheckCard(m_ptMousept))
				{
					m_iCombo++;
					m_iScore += 90;
					m_fFeverDelay += 10;
					m_iFieldCard--;

					if ((*i)->GetCheckCombo())
					{
						m_iScore += m_iBonusCombo * 100;
						m_bCheckCombo = false;
					}

					i = i + 1;
					(*i)->SetFront();
				}
				else
				{
					(*i)->ResetState();
					m_bCheckCombo = false;
					m_iCombo = 0;
					m_iBonusCombo = 0;
					m_fFeverDelay = 0;
					m_pGameUI->SetFailFever();
				}
				return;
			}
		}
	}
}

void CardGameScene::CheckCombo()
{
	if (m_bCheckCombo)
		return;

	if (m_iCombo != 0 && m_iCombo % 5 == 0 || m_pGameUI->GetFeverState() != FEVER_STATE_NORMAL)
	{
		m_bCheckCombo = true;
		if(m_iBonusCombo != MAX_COMBO)
			m_iBonusCombo++;

		for (auto i = m_vcCard.begin(); i != m_vcCard.end(); i++)
		{
			if ((*i)->CheckLife())
			{
				TCHAR str[128];
				wsprintf(str, TEXT("ComboStar_%d.bmp"), m_iBonusCombo - 1);
				Engine::ResManager::GetInstance()->GetBitMap(str);
				(*i)->InitComboStar(Engine::ResManager::GetInstance()->GetBitMap(str));
				break;
			}
		}
	}
	else
		m_bCheckCombo = false;
}