#include "AirplaneScene.h"
#include "Object.h"
#include "GameUI.h"
#include "GameMenu.h"
#include "Bullet.h"
#include "Airplane.h"
#include "Cloud.h"
#include "ComboStar.h"


AirplaneScene::AirplaneScene()
{
}


AirplaneScene::~AirplaneScene()
{
}

void AirplaneScene::Init()
{
	m_pBack[0] = Engine::ResManager::GetInstance()->GetBitMap("AirplaneBack_00.bmp");
	m_pBack[1] = Engine::ResManager::GetInstance()->GetBitMap("AirplaneBack_01.bmp");
	Engine::ResManager::GetInstance()->GetBitMap("Cloud_01.bmp");
	Engine::ResManager::GetInstance()->GetBitMap("Cloud_02.bmp");
	Engine::ResManager::GetInstance()->GetBitMap("Bullet.bmp");
	Engine::ResManager::GetInstance()->GetBitMap("Effect_00.bmp");
	Engine::ResManager::GetInstance()->GetBitMap("Effect_01.bmp");
	Engine::ResManager::GetInstance()->GetBitMap("Effect_02.bmp");
	Engine::ResManager::GetInstance()->GetBitMap("Effect_03.bmp");
	Engine::ResManager::GetInstance()->GetBitMap("Effect_04.bmp");
	Engine::ResManager::GetInstance()->GetBitMap("Effect_05.bmp");

	TCHAR str[128];
	for (int i = 0; i < MAX_COMBO; i++)
	{
		wsprintf(str, TEXT("ComboStar_%d.bmp"), i);
		Engine::ResManager::GetInstance()->GetBitMap(str);
	}

	m_pAirplane = new Airplane;
	m_pAirplane->Init(230, 500);
	m_pGameUI = new GameUI;
	m_pGameUI->Init(DELAY_INDEX_AIRPLANE);
	m_pInfoMenu = new GameMenu;
	m_pInfoMenu->Init(INFO_MENU_POS_X, INFO_MENU_POS_Y, Engine::ResManager::GetInstance()->GetBitMap("AirplaneGameInfo.bmp"), SCENE_END, BUTTON_TYPE_OK);
	m_pInfoMenu->SetButtonPos(150, 600);
	for (int i = 0; i < 4; i++)
	{
		Object* pBitMap = new Cloud;
		if (i == 0 || i == 1)
		{
			POINT Pos = { 0, 200 - (i * 600) };
			pBitMap->Init(Engine::ResManager::GetInstance()->GetBitMap("Cloud_02.bmp"), DOWN, Pos);
		}
		else if (i == 2)
		{
			POINT Pos = { 300, 500 };
			pBitMap->Init(Engine::ResManager::GetInstance()->GetBitMap("Cloud_01.bmp"), DOWN, Pos);
		}
		else if (i == 3)
		{
			POINT Pos = { 300, -300 };
			pBitMap->Init(Engine::ResManager::GetInstance()->GetBitMap("Cloud_01.bmp"), DOWN, Pos);
		}
		else if (i == 4)
		{
			POINT Pos = { 300, -800 };
			pBitMap->Init(Engine::ResManager::GetInstance()->GetBitMap("Cloud_01.bmp"), DOWN, Pos);
		}

		m_vcCloud.push_back(pBitMap);
	}
	
	m_eGameState = GAME_STATE_WAIT;
	m_iCount = 0;
	m_iScore = 0;
	m_iBonusCombo = 0;
	m_iSpeedX = 50;
	m_iSpeedY = 20;
	m_iGenerateStarDelayIndex = STAR_GENERATE_NORMAL_DELAY;
	
	m_fDelay = 0;
	m_fPointDelay = 0;
	m_fGenearteBulletDelay = 0;
	m_fGenerateStarDelay = 0;
	m_fFeverDelay = 0;
	m_fFeverGaugeDelay = 0;
	m_fFeverStateDelay = 0;

	m_bCheckInfo = false;
}


void AirplaneScene::Draw()
{
	if (m_bCheckInfo)
		m_pBack[0]->DrawBitblt(0, 0);
	else
		Engine::ResManager::GetInstance()->GetBitMap("AirplaneBack_Dark.bmp")->Draw(0, 0);

	for (auto i = m_vcCloud.begin(); i != m_vcCloud.end(); i++)
	{
		(*i)->Draw();	
	}
	for (auto i = m_vcStar.begin(); i != m_vcStar.end(); i++)
	{
		(*i)->Draw();
	}
	for (auto i = m_vcBullet.begin(); i != m_vcBullet.end(); i++)
	{
		(*i)->Draw();
	}
	m_pAirplane->Draw();

	if (m_bCheckInfo)
		m_pBack[1]->Draw(0, 0);

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

bool AirplaneScene::Input(float fElapseTime)
{
	InptInfoMenu();

	if (m_eGameState != GAME_STATE_RUN)
		return false;

	POINT pt = Engine::InputManager::GetInstance()->ReturnMouseCusur();

	m_pAirplane->Move((float)pt.x - (AIRPLANE_SIZE_X / 2), (float)pt.y - (AIRPLANE_SIZE_Y / 2));

	return false;
}
void AirplaneScene::Updata(float fElapseTime)
{
	if (UI(fElapseTime))
		return;

	ShowInfoMenu(fElapseTime);

	if (m_eGameState != GAME_STATE_RUN)
		return;

	GenerateBullet(fElapseTime);
	GenerateStar(fElapseTime);
	CheckCrash();
	SettingAuto(fElapseTime);
	SettingScore(fElapseTime);
	ChangeSpeed();
	Action(fElapseTime);
	ResetObject();
}

bool AirplaneScene::UI(float fElapseTime)
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
void AirplaneScene::Release()
{
	for (auto i = m_vcCloud.begin(); i != m_vcCloud.end(); i++)
	{
		SAFE_DELETE((*i));
	}
	for (auto i = m_vcBullet.begin(); i != m_vcBullet.end(); i++)
	{
		SAFE_DELETE((*i));
	}
	for (auto i = m_vcStar.begin(); i != m_vcStar.end(); i++)
	{
		SAFE_DELETE((*i));
	}

	m_vcCloud.clear();
	m_vcBullet.clear();
	m_vcStar.clear();
	m_vcObject.clear();

	m_pGameUI->Release();
	SAFE_DELETE(m_pGameUI);
	SAFE_DELETE(m_pAirplane);
	SAFE_DELETE(m_pInfoMenu);
}

void AirplaneScene::ResetObject()
{
	for (auto i = m_vcStar.begin(); i != m_vcStar.end(); i++)
	{
		if ((*i)->GetState() != OBJECT_STATE_DESTROY)
			m_vcObject.push_back((*i));
		else
			SAFE_DELETE((*i));
	}
	m_vcStar.clear();
	for (auto i = m_vcObject.begin(); i != m_vcObject.end(); i++)
	{
		m_vcStar.push_back((*i));
	}
	m_vcObject.clear();
	for (auto i = m_vcBullet.begin(); i != m_vcBullet.end(); i++)
	{
		if ((*i)->GetState() != OBJECT_STATE_DESTROY)
			m_vcObject.push_back((*i));
		else
			SAFE_DELETE((*i));
	}
	m_vcBullet.clear();
	for (auto i = m_vcObject.begin(); i != m_vcObject.end(); i++)
	{
		m_vcBullet.push_back((*i));
	}
	m_vcObject.clear();
}

void AirplaneScene::SettingAuto(float fElapseTime)
{
	m_fPointDelay = m_fPointDelay + 70 * fElapseTime;
	m_fFeverGaugeDelay = m_fFeverGaugeDelay + 70 * fElapseTime;
	if (m_fPointDelay > 70)
	{
		m_fPointDelay = 0;
		m_iScore += 3;
	}
	if (m_fFeverGaugeDelay > 160)
	{
		m_fFeverGaugeDelay = 0;
		m_fFeverDelay += 10;
	}
}

void AirplaneScene::SettingScore(float fElapseTime)
{
	if (m_pGameUI->GetFeverState() != FEVER_STATE_NORMAL)
	{
		m_fFeverStateDelay = m_fFeverStateDelay + 70 * fElapseTime;
		if (m_fFeverStateDelay > 150)
		{
			m_fFeverStateDelay = 0;
			m_pGameUI->SetFailFever();
		}
	}

	if (m_fFeverDelay > 0)
	{
		m_fFeverDelay = m_fFeverDelay - 100 * fElapseTime;
		m_pGameUI->SetSize(fElapseTime);
	}
}

void AirplaneScene::Action(float fElapseTime)
{
	m_pAirplane->Animation(fElapseTime);

	for (auto i = m_vcCloud.begin(); i != m_vcCloud.end(); i++)
	{
		(*i)->Move(fElapseTime, m_iSpeedX, m_iSpeedY);
		(*i)->MoveTo();
	}
	for (auto i = m_vcStar.begin(); i != m_vcStar.end(); i++)
	{
		(*i)->Move(fElapseTime, m_iSpeedX + 5, m_iSpeedY + 5);
		TCHAR str[128];
		wsprintf(str, TEXT("ComboStar_%d.bmp"), m_iBonusCombo);
		(*i)->ChangeBitMap(Engine::ResManager::GetInstance()->GetBitMap(str));
		(*i)->Effect(fElapseTime);
		(*i)->DestroyObject();
	}
	for (auto i = m_vcBullet.begin(); i != m_vcBullet.end(); i++)
	{
		(*i)->Move(fElapseTime, m_iSpeedX, m_iSpeedY);
		(*i)->DestroyObject();
	}
}

void AirplaneScene::GenerateBullet(float fElaseTime)
{
	if (m_pAirplane->GetState() != AIRPLANE_STATE_LIFE)
		return;

	m_fGenearteBulletDelay = m_fGenearteBulletDelay + 100 * fElaseTime;

	if (m_fGenearteBulletDelay > BULLET_GENERATE_DELAY)
	{
		m_fGenearteBulletDelay = 0;
		Object* pBullet = new Bullet;
		
		int RandomWay;
		while (true)
		{
			RandomWay = rand() % END + DOWN_LEFT;

			if (RandomWay == DOWN_LEFT || RandomWay == DOWN_RIGHT || RandomWay == RIGHT_DOWN || RandomWay == LEFT_DOWN)
				break;
		}
		pBullet->Init(Engine::ResManager::GetInstance()->GetBitMap("Bullet.bmp"),RandomWay, SetRandomPos(RandomWay, OBJECT_TYPE_BULLET));
		m_vcBullet.push_back(pBullet);
	}
}
void AirplaneScene::GenerateStar(float fElaseTime)
{
	if (m_pGameUI->GetFeverState() == FEVER_STATE_FEVER)
		m_iGenerateStarDelayIndex = STAR_GENERATE_FEVER_DELAY;
	else if (m_pGameUI->GetFeverState() == FEVER_STATE_SUPER)
		m_iGenerateStarDelayIndex = STAR_GENERATE_SPUER_DELAY;
	else if (m_pGameUI->GetFeverState() == FEVER_STATE_ULTRA)
		m_iGenerateStarDelayIndex = STAR_GENERATE_ULTRA_DELAY;
	else
		m_iGenerateStarDelayIndex = STAR_GENERATE_NORMAL_DELAY;

	if (m_pAirplane->GetState() != AIRPLANE_STATE_LIFE)
		return;

	m_fGenerateStarDelay = m_fGenerateStarDelay + 100 * fElaseTime;

	if (m_fGenerateStarDelay > m_iGenerateStarDelayIndex)
	{
		m_fGenerateStarDelay = 0;
		Object* pStar = new ComboStar;

		int RandomWay = rand() % END + DOWN_LEFT;
		pStar->Init(Engine::ResManager::GetInstance()->GetBitMap("ComboStar_0.bmp"), RandomWay, SetRandomPos(RandomWay, OBJECT_TYPE_STAR));
		pStar->InitEffect();
		m_vcStar.push_back(pStar);
	}
}

void AirplaneScene::CheckCrash()
{
	RECT rcResult;
	for (auto i = m_vcBullet.begin(); i != m_vcBullet.end(); i++)
	{
		if ((*i)->GetState() == OBJECT_STATE_LIFE)
		{
			if (IntersectRect(&rcResult, &(*i)->GetRect(), &m_pAirplane->GetRect()))
			{
				if (m_pAirplane->GetState() == AIRPLANE_STATE_LIFE)
				{
					m_pAirplane->Explosion();
					m_pGameUI->SetFailFever();
					for (auto j = m_vcBullet.begin(); j != m_vcBullet.end(); j++)
						(*j)->Hide();
					for (auto j = m_vcStar.begin(); j != m_vcStar.end(); j++)
						(*j)->Hide();

					m_iBonusCombo = 0;
					m_fFeverDelay = 0;

					break;
				}
			}
		}
	}
	for (auto i = m_vcStar.begin(); i != m_vcStar.end(); i++)
	{
		if ((*i) != NULL)
		{
			if ((*i)->GetState() == OBJECT_STATE_LIFE)
			{
				if (IntersectRect(&rcResult, &(*i)->GetRect(), &m_pAirplane->GetRect()))
				{
					if (m_pAirplane->GetState() == AIRPLANE_STATE_LIFE)
					{
						m_fFeverStateDelay = 0;
						m_iBonusCombo++;
						m_iScore += SCORE * m_iBonusCombo;
						m_fFeverDelay += 30;
						(*i)->Crash();
						break;
					}
				}
			}
		}
	}
}

void AirplaneScene::ChangeSpeed()
{
	if (m_pGameUI->GetFeverState() == FEVER_STATE_FEVER)
	{
		m_iSpeedX = 60;
		m_iSpeedY = 30;
	}
	else if (m_pGameUI->GetFeverState() == FEVER_STATE_SUPER)
	{
		m_iSpeedX = 70;
		m_iSpeedY = 40;
	}
	else if (m_pGameUI->GetFeverState() == FEVER_STATE_ULTRA)
	{
		m_iSpeedX = 80;
		m_iSpeedY = 50;
	}
	else
	{
		m_iSpeedX = 50;
		m_iSpeedY = 20;
	}
}

POINT AirplaneScene::SetRandomPos(int RandomWay, int Type)
{
	int Index;
	float fX;
	float fY;

	int iBitMapSize;

	if (Type == OBJECT_TYPE_BULLET)
		iBitMapSize = BULLET_SIZE;
	else if(Type == OBJECT_TYPE_STAR)
		iBitMapSize = STAR_SIZE;

	if (RandomWay == DOWN_LEFT)
	{
		Index = rand() % FIELD_RIGHT + (FIELD_RIGHT / 2);
		fX = (float)Index;
		Index = FIELD_TOP;
		fY = (float)Index - iBitMapSize;
	}
	else if (RandomWay == DOWN_RIGHT)
	{
		Index = rand() % (FIELD_RIGHT / 2) + FIELD_LEFT;
		fX = (float)Index;
		Index = FIELD_TOP;
		fY = (float)Index - iBitMapSize;
	}
	else if (RandomWay == LEFT_UP)
	{
		Index = FIELD_RIGHT;
		fX = (float)Index + iBitMapSize;
		Index = rand() % FIELD_BOTTOM + (FIELD_BOTTOM / 2);
		fY = (float)Index;
	}
	else if (RandomWay == LEFT_DOWN)
	{
		Index = FIELD_RIGHT;
		fX = (float)Index + iBitMapSize;
		Index = rand() % (FIELD_BOTTOM / 2) + FIELD_TOP;
		fY = (float)Index;
	}
	else if (RandomWay == RIGHT_UP)
	{
		Index = FIELD_LEFT;
		fX = (float)Index - iBitMapSize;
		Index = rand() % FIELD_BOTTOM + (FIELD_BOTTOM / 2);
		fY = (float)Index;
	}
	else if (RandomWay == RIGHT_DOWN)
	{
		Index = FIELD_LEFT;
		fX = (float)Index - iBitMapSize;
		Index = rand() % (FIELD_BOTTOM / 2) + FIELD_TOP;
		fY = (float)Index;
	}

	POINT Pos = { (LONG)fX, (LONG)fY };
	return Pos;
}

void AirplaneScene::Reset()
{
	m_pAirplane->Explosion();
	m_pGameUI->SetFailFever();
	for (auto j = m_vcBullet.begin(); j != m_vcBullet.end(); j++)
		(*j)->Hide();
	for (auto j = m_vcStar.begin(); j != m_vcStar.end(); j++)
		(*j)->Hide();

	m_iBonusCombo = 0;
	m_fFeverDelay = 0;
}

void AirplaneScene::InptInfoMenu()
{
	POINT pt = { Engine::InputManager::GetInstance()->GetMousePos().x, Engine::InputManager::GetInstance()->GetMousePos().y };
	RECT rc = m_pInfoMenu->GetButtonSize();
	if (PtInRect(&rc, pt))
		m_pInfoMenu->ChangeButtonState();
}
void AirplaneScene::ShowInfoMenu(float fElapseTime)
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