#include "GameManager.h"
#include "Player.h"
#include "BackGround.h"
#include "Enemy.h"
#include "JarEnemy.h"
#include "FireRing.h"
#include "ShowBitMap.h"
#include "MainIntro.h"

#include "resource.h"

GameManager::GameManager()
{
}


GameManager::~GameManager()
{
}

GameManager* GameManager::m_Instance = NULL;

GameManager* GameManager::GetInstance()
{
	if (m_Instance == NULL)
	{
		m_Instance = new GameManager;
	}

	return m_Instance;
}


void GameManager::ResetGame()
{
	m_pPlayer->Reset();

	int iLen = 0;
	
	while (true)
	{
		auto Back = m_vcBackGround.begin();

		if ((*Back)->GetPoint().x == 0)
			break;

		for (auto i = m_vcBackGround.begin(); i != m_vcBackGround.end(); i++)
		{
			(*i)->MoveScroll(VK_LEFT);
			(*i)->Reset();
		}
	}
	for (auto i = m_vcEnemy.begin(); i != m_vcEnemy.end(); i++)
	{
		if ((*i)->GetNum() == 2)
		{
			(*i)->MoveSet();
			iLen += 5;
			(*i)->Reset(iLen * 130.0);
		}
		else if ((*i)->GetNum() == 4)
		{
			(*i)->MoveSet();
			(*i)->Reset(m_pEndTile->GetPoint().x - FRONT);
		}
		else
			(*i)->StopEnemy();
	}
	m_iGameState = GAME_STATE_MAIN;
	m_iBonusScore = 5000;
	m_iLife = 4;

	m_iEndDelay = 0;
	m_iDelay = 0;
	m_iScoreDelay = 0;
	m_iShowLength = 0;
	m_iEnemyNum = 0;
	m_iGenerateDelay = 0;
}
void GameManager::Init(HWND hWnd, HINSTANCE hInst, HDC hdc)
{
	m_iLife = 4;
	m_iScore = 0;
	m_iGameState = GAME_STATE_MAIN;
	m_iBonusScore = 5000;

	m_iEndDelay = 0;
	m_iDelay = 0;
	m_iScoreDelay = 0;
	m_iShowLength = 0;
	m_iEnemyNum = 0;
	m_iGenerateDelay = 0;
	m_iObject = 72;
	m_iSaveMiter = RES_TYPE_MITER_100;

	m_hMemDC[0] = CreateCompatibleDC(hdc); // 0. 더블버퍼 1. 배경
	m_hBitMap[0] = CreateCompatibleBitmap(hdc, 600, 314); // 244
	m_hOldBitMap[0] = (HBITMAP)SelectObject(m_hMemDC[0], m_hBitMap[0]);

	/*
	m_hMemDC[1] = CreateCompatibleDC(m_hMemDC[0]);
	m_hBitMap[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP29));
	m_hOldBitMap[1] = (HBITMAP)SelectObject(m_hMemDC[1], m_hBitMap[1]);
	*/
	m_pResource = new ResManager;
	m_pResource->Init(m_hMemDC[0], hInst);

	m_pPlayer = new Player; // 플레이어
	m_pPlayer->Init(m_pResource->GetBitMap((RES_TYPE)((int)RES_TYPE_PLAYER_00)), m_pResource->GetBitMap((RES_TYPE)((int)RES_TYPE_PLAYER_01)), m_pResource->GetBitMap((RES_TYPE)((int)RES_TYPE_PLAYER_02)), m_pResource->GetBitMap((RES_TYPE)((int)RES_TYPE_DIE)), m_pResource->GetBitMap((RES_TYPE)((int)RES_TYPE_WIN_00)), m_pResource->GetBitMap((RES_TYPE)((int)RES_TYPE_WIN_01)), PLAYER_POS_X, PLAYER_POS_Y); // 30 117
	
	int iLen = STAR_TYPE_00;
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (i == 0 || i == 15 || j == 0 || j == 7)
			{
				MainIntro* pMain = new MainIntro;
				pMain->Init(m_pResource->GetBitMap((RES_TYPE)(RES_TYPE_STAR_00)), m_pResource->GetBitMap((RES_TYPE)(RES_TYPE_STAR_01)), m_pResource->GetBitMap((RES_TYPE)(RES_TYPE_STAR_02)), (i * STAR_SIZE_X) + 170, (j * STAR_SIZE_Y) + 70, iLen);
				m_vcMainIntro.push_back(pMain);
				iLen++;
				if (iLen == 3)
					iLen = STAR_TYPE_00;
			}
		}
	}

	int MiterCount = 0;
	BackGround* m_pBackGround;
	for (int i = 0; i < m_iObject; i++)
	{
		if (i % 6 == 0)
		{
			m_pBackGround = new BackGround;
			m_pBackGround->Init(m_pResource->GetBitMap((RES_TYPE)(RES_TYPE_BACK_DECO)), m_pResource->GetBitMap((RES_TYPE)(RES_TYPE_BACK_DECO)), i * BACK_NORMAL_X, 0, RES_TYPE_BACK_DECO);
			m_vcBackGround.push_back(m_pBackGround);
		}
		else
		{
			m_pBackGround = new BackGround;
			m_pBackGround->Init(m_pResource->GetBitMap((RES_TYPE)(RES_TYPE_BACK_NORMAL00)), m_pResource->GetBitMap((RES_TYPE)(RES_TYPE_BACK_NORMAL01)), i * BACK_NORMAL_X, 0, RES_TYPE_BACK_NORMAL01);
			m_vcBackGround.push_back(m_pBackGround);
		}

		m_pBackGround = new BackGround;
		m_pBackGround->Init(m_pResource->GetBitMap((RES_TYPE)(RES_TYPE_BACK)), m_pResource->GetBitMap((RES_TYPE)(RES_TYPE_BACK)), i * BACK_X , BACK_NORMAL_Y, RES_TYPE_BACK);
		m_vcBackGround.push_back(m_pBackGround);

		if (i % 7 == 0)
		{
			m_pBackGround = new BackGround;
			m_pBackGround->Init(m_pResource->GetBitMap((RES_TYPE)(RES_TYPE_MITER_100 + MiterCount)), m_pResource->GetBitMap((RES_TYPE)(RES_TYPE_MITER_100 + MiterCount)), i * 64, 200, RES_TYPE_MITER_100 + MiterCount); // 200
			m_vcBackGround.push_back(m_pBackGround);
			MiterCount++;
			
			if (RES_TYPE_MITER_100 + MiterCount == RES_TYPE_MITER_00)
				m_pMiter = m_pBackGround;
		}

		if (i == m_iObject - 4)
		{
			m_pBackGround = new BackGround;
			m_pBackGround->Init(m_pResource->GetBitMap((RES_TYPE)(RES_TYPE_OBJECT_END)), m_pResource->GetBitMap((RES_TYPE)(RES_TYPE_OBJECT_END)), i * BACK_X, 130, RES_TYPE_OBJECT_END);
			m_pEndTile = m_pBackGround;
			m_vcBackGround.push_back(m_pBackGround);
		}
	}

	m_pScoreBoard = new ShowBitMap; // 스코어 보드
	m_pScoreBoard->Init(m_pResource->GetBitMap((RES_TYPE)(RES_TYPE_SCORDBOARD)), 0, 0);

	m_pTitleLogo = new ShowBitMap; // 타이틀 로고
	m_pTitleLogo->Init(m_pResource->GetBitMap((RES_TYPE)(RES_TYPE_TITLE_LOGO)), 180, 90);

	m_pShowStage = new ShowBitMap; // 스테이지 
	m_pShowStage->Init(m_pResource->GetBitMap((RES_TYPE)(RES_TYPE_BLACKBACKGROUND)), 0, 0);

	GenerateJarEnemy(); // 항아리 물체 
}
void GameManager::Draw(HDC hdc)
{
	BITMAP bmp_info;
	GetObject(m_pShowStage->ReturnHbitmap(), sizeof(bmp_info), &bmp_info);
	BitBlt(m_hMemDC[0], m_pShowStage->GetPoint().x, m_pShowStage->GetPoint().y, (int)bmp_info.bmWidth, (int)bmp_info.bmHeight, m_pShowStage->Draw(), 0, 0, SRCCOPY);

	TCHAR str[128];
	SetBkMode(m_hMemDC[0], TRANSPARENT);
	SetTextColor(m_hMemDC[0], RGB(255, 255, 255));
	if (m_iGameState == GAME_STATE_MAIN)
	{
		MainDraw();
		BitBlt(hdc, 0, 0, 1280, 720, m_hMemDC[0], 0, 0, SRCCOPY);
		return;
	}
	else if (m_iGameState == GAME_STATE_WAIT)
	{
		GetObject(m_pShowStage->ReturnHbitmap(), sizeof(bmp_info), &bmp_info);
		BitBlt(m_hMemDC[0], m_pShowStage->GetPoint().x, m_pShowStage->GetPoint().y, (int)bmp_info.bmWidth, (int)bmp_info.bmHeight, m_pShowStage->Draw(), 0, 0, SRCCOPY);
		wsprintf(str, TEXT("STAGE 01"));
		TextOut(m_hMemDC[0], 255, 125, str, lstrlen(str));
		
		BitBlt(hdc, 0, 0, 1280, 720, m_hMemDC[0], 0, 0, SRCCOPY);

		if (m_iDelay == 120)
		{
			m_iGameState = GAME_STATE_RUN;
			m_iDelay = 0;
		}
		else
			m_iDelay++;
		return;
	}

	if (m_iGameState != GAME_STATE_RUN)
		return;

	GetObject(m_pScoreBoard->ReturnHbitmap(), sizeof(bmp_info), &bmp_info); //스코어보드
	BitBlt(m_hMemDC[0], m_pScoreBoard->GetPoint().x, m_pScoreBoard->GetPoint().y, (int)bmp_info.bmWidth, (int)bmp_info.bmHeight, m_pScoreBoard->Draw(), 0, 0, SRCCOPY);

	for (auto i = m_vcBackGround.begin(); i != m_vcBackGround.end(); i++)
	{
		BITMAP bmp_info;
		GetObject((*i)->ReturnHbitmap(), sizeof(bmp_info), &bmp_info);
		if ((*i)->GetPoint().x - (SHOW_DISTANCE_ONE - m_iShowLength) < (*i)->GetPoint().x && (*i)->GetPoint().x + (SHOW_DISTANCE_TWO - m_iShowLength) > (*i)->GetPoint().x)
			TransparentBlt(m_hMemDC[0], (*i)->GetPoint().x, (*i)->GetPoint().y, (int)bmp_info.bmWidth, (int)bmp_info.bmHeight, (*i)->Draw(), 0, 0, (int)bmp_info.bmWidth, (int)bmp_info.bmHeight, RGB(255, 0, 255));
	}
	for (auto i = m_vcEnemy.begin(); i != m_vcEnemy.end(); i++)
	{
		if ((*i)->GetDelete() != true)
		{
			GetObject((*i)->ReturnHbitmap(), sizeof(bmp_info), &bmp_info);
			if ((*i)->GetNum() != 1)
			{
				if ((*i)->GetPoint().x - (SHOW_DISTANCE_ONE - m_iShowLength) < (*i)->GetPoint().x && (*i)->GetPoint().x + (SHOW_DISTANCE_TWO - m_iShowLength) > (*i)->GetPoint().x)
					TransparentBlt(m_hMemDC[0], (*i)->GetPoint().x, (*i)->GetPoint().y, (int)bmp_info.bmWidth, (int)bmp_info.bmHeight, (*i)->Draw(), 0, 0, (int)bmp_info.bmWidth, (int)bmp_info.bmHeight, RGB(255, 0, 255));
			}
		}
		if ((*i)->GetNum() == 3 && (*i)->GetScore())
		{
			wsprintf(str, TEXT("500"));
			TextOut(m_hMemDC[0], m_pPlayer->GetPoint().x + (PLAYER_X / 2), m_pPlayer->GetPoint().y - 10, str, lstrlen(str));
			m_iDelay++;

			if (m_iDelay == 20)
			{
				(*i)->DeleteScore();
				m_iDelay = 0;
			}
		}
	}

	GetObject(m_pPlayer->ReturnHbitmap(), sizeof(bmp_info), &bmp_info); // 플레이어
	TransparentBlt(m_hMemDC[0], m_pPlayer->GetPoint().x, m_pPlayer->GetPoint().y, (int)bmp_info.bmWidth, (int)bmp_info.bmHeight, m_pPlayer->Draw(), 0, 0, (int)bmp_info.bmWidth, (int)bmp_info.bmHeight, RGB(255, 0, 255));

	for (auto i = m_vcEnemy.begin(); i != m_vcEnemy.end(); i++)
	{
		if ((*i)->GetDelete() != true)
		{
			GetObject((*i)->ReturnHbitmap(), sizeof(bmp_info), &bmp_info);
			if ((*i)->GetNum() == 1)
			{
				if ((*i)->GetPoint().x - (SHOW_DISTANCE_ONE - m_iShowLength) < (*i)->GetPoint().x && (*i)->GetPoint().x + (SHOW_DISTANCE_TWO - m_iShowLength) > (*i)->GetPoint().x)
					TransparentBlt(m_hMemDC[0], (*i)->GetPoint().x, (*i)->GetPoint().y, (int)bmp_info.bmWidth, (int)bmp_info.bmHeight, (*i)->Draw(), 0, 0, (int)bmp_info.bmWidth, (int)bmp_info.bmHeight, RGB(255, 0, 255));
			}
		}
	}

	for (int i = 0; i < m_iLife; i++)
	{
		HDC hdcRes = m_pResource->GetBitMap((RES_TYPE)(RES_TYPE_ICON))->Draw();
		TransparentBlt(m_hMemDC[0], (i * ICON_X) + 513, 30, ICON_X, ICON_Y, hdcRes, 0, 0, ICON_X, ICON_Y, RGB(255, 0, 255));
	}

	SetTextColor(m_hMemDC[0], RGB(255, 0, 0));
	wsprintf(str, TEXT("Bouns - "));
	TextOut(m_hMemDC[0], 200, 25, str, lstrlen(str));

	SetTextColor(m_hMemDC[0], RGB(255, 255, 255));

	wsprintf(str, TEXT("%d"), m_iBonusScore);
	TextOut(m_hMemDC[0], 260, 25, str, lstrlen(str));
	wsprintf(str, TEXT("1P - %d"), m_iScore);
	TextOut(m_hMemDC[0], 35, 25, str, lstrlen(str));
	wsprintf(str, TEXT("STAGE 01"));
	TextOut(m_hMemDC[0], 515, 15, str, lstrlen(str));


	BitBlt(hdc, 0, 0, 1280, 720, m_hMemDC[0], 0, 0, SRCCOPY);
}
void GameManager::Release()
{
	m_pResource->Release();

	for (auto i = m_vcBackGround.begin(); i != m_vcBackGround.end(); i++)
	{
		SAFE_DELETE((*i));
	}
	for (auto i = m_vcEnemy.begin(); i != m_vcEnemy.end(); i++)
	{
		SAFE_DELETE((*i));
	}
	for (auto i = m_vcMainIntro.begin(); i != m_vcMainIntro.end(); i++)
	{
		SAFE_DELETE((*i));
	}
	SAFE_DELETE(m_pResource);
	SAFE_DELETE(m_pPlayer);
	SAFE_DELETE(m_pScoreBoard);
	SAFE_DELETE(m_pShowStage);
	SAFE_DELETE(m_pTitleLogo);

	delete m_Instance;
}


void GameManager::MainDraw()
{
	BITMAP bmp_info;
	TCHAR str[128];

	SetTextColor(m_hMemDC[0], RGB(255, 255, 255));
	wsprintf(str, TEXT("『SPACE BAR』PRESS TO START!!"));
	TextOut(m_hMemDC[0], 160, 220, str, lstrlen(str));

	GetObject(m_pTitleLogo->ReturnHbitmap(), sizeof(bmp_info), &bmp_info); //타이틀 로고
	BitBlt(m_hMemDC[0], m_pTitleLogo->GetPoint().x, m_pTitleLogo->GetPoint().y, (int)bmp_info.bmWidth, (int)bmp_info.bmHeight, m_pTitleLogo->Draw(), 0, 0, SRCCOPY);

	for (auto i = m_vcMainIntro.begin(); i != m_vcMainIntro.end(); i++)
	{
		GetObject((*i)->ReturnHbitmap(), sizeof(bmp_info), &bmp_info);
		TransparentBlt(m_hMemDC[0], (*i)->GetPoint().x, (*i)->GetPoint().y, (int)bmp_info.bmWidth, (int)bmp_info.bmHeight, (*i)->Draw(), 0, 0, (int)bmp_info.bmWidth, (int)bmp_info.bmHeight, RGB(255, 0, 255));
	}
}

void GameManager::MainMenu()
{
	if (m_iGameState != GAME_STATE_MAIN)
		return;

	for (auto i = m_vcMainIntro.begin(); i != m_vcMainIntro.end(); i++)
	{
		(*i)->Motion();
	}
	m_iScore = 0;

	if (GetKeyState(VK_SPACE) & 0x8000)
		m_iGameState = GAME_STATE_WAIT;
}

bool GameManager::Updata(HWND hWnd)
{
	MainMenu();

	if (m_iGameState != GAME_STATE_RUN)
		return false;

	CrashEnemy();

	Input(hWnd);
	SaveDistance();
	EndLine();
	CrashCheck();
	MoveScroll();

	GenerateEnemy();

	if (m_iLife == 0 || m_iBonusScore == 0)
	{
		m_iDelay++;
		if (m_iDelay == 15)
		{
			m_iGameState = GAME_STATE_MAIN;
			return true;
		}
	}

	return false;
}

void GameManager::Input(HWND hWnd)
{
	if (CheckPlayerState())
		return;

	if (GetKeyState(VK_LEFT) & 0x8000)
		m_iKeyType = VK_LEFT;
	else if (GetKeyState(VK_RIGHT) & 0x8000)
		m_iKeyType = VK_RIGHT;
	else if (GetKeyState(VK_SPACE) & 0x8000)
		m_iKeyType = VK_SPACE;
	else
		m_iKeyType = NULL;

	m_iKeyType = m_pPlayer->Jump(m_iKeyType);
}

void GameManager::EndLine() // 마지막 라인
{
	if (CheckPlayerState())
		return;

	auto BackFirst = m_vcBackGround.begin();

	if (m_iKeyType == VK_LEFT || m_iKeyType == PLAYER_STATE_LEFTJUMP)
	{
		if ((*BackFirst)->GetPoint().x == 0)
			return;
	}

	if (m_bEndLine) // 마지막 라인
	{
		m_pPlayer->Move(m_iKeyType); // 플레이어의 x, y값 변경

		if (m_pMiter->GetPoint().x + MITER_X > m_pPlayer->GetPoint().x)
			m_bEndLine = false;
	}
	else
	{
		if (m_pMiter->GetPoint().x + MITER_X < m_pPlayer->GetPoint().x)
			m_bEndLine = true;

		for (auto i = m_vcBackGround.begin(); i != m_vcBackGround.end(); i++)
		{
			(*i)->MoveScroll(m_iKeyType);
		}
	}
}

void GameManager::MoveScroll()
{
	if (m_pPlayer->GetWin() && m_iBonusScore != 0)
	{
		m_pPlayer->WinMotion();
		for (auto i = m_vcBackGround.begin(); i != m_vcBackGround.end(); i++)
		{
			if ((*i)->GetType() == RES_TYPE_BACK_NORMAL01)
			{
				(*i)->MoveMotion();
			}
		}
		m_iBonusScore -= 10;
		m_iScore += 10;
	}

	if (CheckPlayerState())
		return;

	for (auto i = m_vcEnemy.begin(); i != m_vcEnemy.end(); i++) // 장애물 스크롤
	{
		(*i)->MoveScroll(m_iKeyType);
		(*i)->MoveMotion();

		if (m_bEndLine)
		{
			if ((*i)->GetPoint().x < m_pMiter->GetPoint().x - 400)
				(*i)->StopEnemy();
		}
		else
		{
			if ((*i)->GetPoint().x < m_pPlayer->GetPoint().x - 130)
				(*i)->StopEnemy();
		}
	}

	if (m_iScoreDelay == 25) // 점수 카운트
	{
		m_iBonusScore -= 10;
		m_iScoreDelay = 0;
	}

	m_iScoreDelay++;

	if (m_iBonusScore == 0) // 보너스가 0점일때 점수X
		m_iScoreDelay = 0;
}

void GameManager::GenerateEnemy() // 장애물 생성
{
	if (CheckPlayerState())
		return;
	if (m_bEndLine)
		return;

	if (m_iKeyType == VK_LEFT || m_iKeyType == PLAYER_STATE_LEFTJUMP)
		m_iGenerateDelay--;
	else if (m_iKeyType == VK_SPACE || m_iKeyType == NULL)
		m_iGenerateDelay++;
	else if (m_iKeyType == VK_RIGHT || m_iKeyType == PLAYER_STATE_RIGHTJUMP)
		m_iGenerateDelay += 2;

	if (m_iGenerateDelay < 300)
		return;

	for (int i = 0; i < 2; i++) // 장애물 생성
	{
		Enemy* pEnemy = new FireRing;
		if (m_iEnemyNum % 5 == 0 && m_iEnemyNum != 0)
			pEnemy->Init(m_pResource->GetBitMap((RES_TYPE)((int)(RES_TYPE_CASH_A00 + i))), m_pResource->GetBitMap((RES_TYPE)((int)(RES_TYPE_CASH_B00 + i))), (m_pPlayer->GetPoint().x + 530) + (i * ENEMY_X), 15, i);
		else
			pEnemy->Init(m_pResource->GetBitMap((RES_TYPE)((int)(RES_TYPE_ENEMY_A00 + i))), m_pResource->GetBitMap((RES_TYPE)((int)(RES_TYPE_ENEMY_B00 + i))), (m_pPlayer->GetPoint().x + 530) + (i * ENEMY_X), 15, i);

		m_vcEnemy.push_back(pEnemy);
	}
	if (m_iEnemyNum % 5 == 0 && m_iEnemyNum != 0) // 캐쉬 생성
	{
		Enemy* pCash = new FireRing;
		pCash->Init(m_pResource->GetBitMap((RES_TYPE)((int)RES_TYPE_CASH)), m_pResource->GetBitMap((RES_TYPE)((int)RES_TYPE_CASH)), (m_pPlayer->GetPoint().x + 530) + 15, 25, 3);
		m_vcEnemy.push_back(pCash);
	}

	m_iEnemyNum++;
	m_iGenerateDelay = 0;
}

void GameManager::GenerateJarEnemy()
{
	Enemy* pEnemy;
	for (int i = 0; i < m_iObject; i++)
	{
		if (m_pMiter->GetPoint().x < i * 130)
			break;
		if (i % 5 == 0 && i != 0)
		{
			pEnemy = new JarEnemy;
			pEnemy->Init(m_pResource->GetBitMap((RES_TYPE)((int)RES_TYPE_FRONT_00)), m_pResource->GetBitMap((RES_TYPE)((int)(RES_TYPE_FRONT_01))), (i * 130), 133, 2);
			m_vcEnemy.push_back(pEnemy);
		}
	}

	for (auto i = m_vcBackGround.begin(); i != m_vcBackGround.end(); i++)
	{
		if ((*i)->GetType() == RES_TYPE_OBJECT_END)
		{
			pEnemy = new JarEnemy;
			pEnemy->Init(m_pResource->GetBitMap((RES_TYPE)((int)RES_TYPE_FRONT_00)), m_pResource->GetBitMap((RES_TYPE)((int)(RES_TYPE_FRONT_01))), (*i)->GetPoint().x - FRONT, 133, 4);
			m_vcEnemy.push_back(pEnemy);
			return;
		}
	}
}

void GameManager::CrashCheck() // 충돌체크
{
	if (CheckPlayerState())
		return;

	POINT PlayerPos = m_pPlayer->GetPoint();
	RECT rcResult;
	RECT rcOne = { PlayerPos.x + 10, PlayerPos.y , PlayerPos.x + PLAYER_X , PlayerPos.y + PLAYER_Y };

	for (auto i = m_vcEnemy.begin(); i != m_vcEnemy.end(); i++)
	{
		POINT EnemyPos = (*i)->GetPoint();

		BITMAP bmp_info;
		GetObject((*i)->ReturnHbitmap(), sizeof(bmp_info), &bmp_info); //스코어보드
		if ((*i)->GetNum() == 2 || (*i)->GetNum() == 4 )
		{
			RECT rcTwo = { EnemyPos.x + 20, EnemyPos.y + 15, EnemyPos.x + (int)bmp_info.bmWidth, EnemyPos.y + (int)bmp_info.bmHeight };
			if (IntersectRect(&rcResult, &rcOne, &rcTwo))
			{
				m_pPlayer->TouchCheckObstacle();
				m_iLife--;
				return;
			}
			else if (EnemyPos.x > PlayerPos.x && EnemyPos.x + (int)bmp_info.bmWidth < PlayerPos.x + PLAYER_X && EnemyPos.y > PlayerPos.y + PLAYER_Y) // 장애물 점수
				(*i)->PlusScore();
		}
		else if ((*i)->GetNum() == 3)
		{
			RECT rcTwo = { EnemyPos.x + 10, EnemyPos.y, EnemyPos.x + (int)bmp_info.bmWidth - 10, EnemyPos.y + (int)bmp_info.bmHeight };
			if (IntersectRect(&rcResult, &rcOne, &rcTwo))
			{
				if ((*i)->GetScore() != true)
				{
					(*i)->StopEnemy();
					(*i)->PlusScore();
					m_iScore += 500;
				}	
			}
		}
		else
		{
			if ((*i)->GetNum() == 0)
			{
				RECT rcTwo = { EnemyPos.x + 15, EnemyPos.y, EnemyPos.x + (int)bmp_info.bmWidth, EnemyPos.y + (int)bmp_info.bmHeight - 15 };
				if (IntersectRect(&rcResult, &rcOne, &rcTwo))
				{
					if (PlayerPos.y + PLAYER_Y >= EnemyPos.y + (int)bmp_info.bmHeight)
					{
						m_pPlayer->TouchCheckObstacle();
						m_iLife--;
						return;
					}
					(*i)->PlusScore();
				}
			}
			else if ((*i)->GetNum() == 1)
			{
				RECT rcTwo = { EnemyPos.x, EnemyPos.y, EnemyPos.x + (int)bmp_info.bmWidth - 15, EnemyPos.y + (int)bmp_info.bmHeight - 15 };
				if (IntersectRect(&rcResult, &rcOne, &rcTwo))
				{
					if (PlayerPos.y + PLAYER_Y >= EnemyPos.y + (int)bmp_info.bmHeight)
					{
						m_pPlayer->TouchCheckObstacle();
						m_iLife--;
						return;
					}
					(*i)->PlusScore();
				}
			}
		}
	}
	for (auto i = m_vcEnemy.begin(); i != m_vcEnemy.end(); i++)
	{
		if ((*i)->GetScore())
		{
			if (m_pPlayer->GetPoint().y == PLAYER_POS_Y)
			{
				(*i)->DeleteScore();
				if ((*i)->GetNum() == 2)
					m_iScore += 200;
				else
					m_iScore += 50;
			}
		}
	}

	for (auto i = m_vcBackGround.begin(); i != m_vcBackGround.end(); i++) // 결승선 체크
	{
		if ((*i)->GetType() == RES_TYPE_OBJECT_END)
		{
			POINT BackPos = (*i)->GetPoint();
			RECT rcTwo = { BackPos.x, BackPos.y, BackPos.x + END_X, BackPos.y + END_Y };

			if (IntersectRect(&rcResult, &rcOne, &rcTwo))
				m_pPlayer->CheckWin(BackPos.x + 5, BackPos.y);
		}
	}
}

void GameManager::SaveDistance()
{
	if (CheckPlayerState())
		return;
	if (m_bEndLine)
		return;

	auto BackFirst = m_vcBackGround.begin();

	int MiterCount = 0;
	for (int i = 0; i < m_iObject; i++)
	{
		if (i % 7 == 0)
		{
			if ((*BackFirst)->GetPoint().x < -(BACK_NORMAL_X * i))
				m_iSaveMiter = RES_TYPE_MITER_100 + MiterCount;

			MiterCount++;
		}
	}

}

void GameManager::CrashEnemy()
{
	if (m_pPlayer->GetDie())
		m_iEndDelay++;
	else
		return;

	if (m_iEndDelay == 250)
	{
		for (auto i = m_vcEnemy.begin(); i != m_vcEnemy.end(); i++)
		{
			if ((*i)->GetPoint().x < m_pPlayer->GetPoint().x + PLAYER_X)
				(*i)->StopEnemy();

			if ((*i)->GetNum() != 2)
				(*i)->StopEnemy();
		}

		for (auto i = m_vcBackGround.begin(); i != m_vcBackGround.end(); i++) // 탈락지점 이동
		{
			if ((*i)->GetType() == m_iSaveMiter)
			{
				while (true)
				{
					if (m_pPlayer->GetPoint().x == (*i)->GetPoint().x + MITER_X)
						break;

					for (auto j = m_vcBackGround.begin(); j != m_vcBackGround.end(); j++)
					{
						(*j)->MoveScroll(VK_LEFT);
					}
					for (auto j = m_vcEnemy.begin(); j != m_vcEnemy.end(); j++)
					{
						if((*j)->GetNum() == 2)
							(*j)->MoveScroll(VK_LEFT);
						else if ((*j)->GetNum() == 4)
							(*j)->Reset(m_pEndTile->GetPoint().x - FRONT);
					}
				}
			}
		}
			
		m_iBonusScore = 5000;
		m_iGenerateDelay = 0;
		m_pPlayer->TouchCheckObstacle();
		m_iEndDelay = 0;
		m_iGameState = GAME_STATE_WAIT;
	}
}

bool GameManager::CheckPlayerState()
{
	if (m_pPlayer->GetWin())
		return true;
	if (m_pPlayer->GetDie())
		return true;

	return false;
}