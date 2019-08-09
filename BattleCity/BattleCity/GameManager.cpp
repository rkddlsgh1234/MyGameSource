#include "GameManager.h"
#include "Tile.h"
#include "ResManager.h"
#include "MapToolManager.h"
#include "Tank.h"
#include "Missile.h"
#include "Cursur.h"
#include "Enemy.h"
#include "Player.h"
#include "ShowBitMap.h"

GameManager::GameManager()
{
	m_dwLastTime = GetTickCount();
	m_dwCurTime = GetTickCount();
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

void GameManager::Init(HWND hWnd, HINSTANCE g_hInst)
{
	srand(time(NULL));
	m_hWnd = hWnd;
	m_hInst = g_hInst;
	HDC hdc = GetDC(hWnd);

	m_fTitlePosY = 600;
	m_fDelay = 0;
	m_fGenerationDelay = 400;

	m_iTileNum = 15;
	m_iEnemy = 16;
	m_iLife = 2;
	m_iScore = 0;
	m_iFieldEnemy = 0;
	
	m_iGameState = GAME_STATE_MAIN;

	m_hMemDC[0] = CreateCompatibleDC(hdc); // 0. 더블버퍼 1. 배경
	m_hBitMap[0] = CreateCompatibleBitmap(hdc, 600, 440); // 660 500
	m_hOldBitMap[0] = (HBITMAP)SelectObject(m_hMemDC[0], m_hBitMap[0]);

	m_hMemDC[1] = CreateCompatibleDC(m_hMemDC[0]);
	m_hBitMap[1] = CreateCompatibleBitmap(hdc, 600, 440);
	m_hOldBitMap[1] = (HBITMAP)SelectObject(m_hMemDC[1], m_hBitMap[1]);

	MapToolManager::GetInstance()->Init(m_hWnd, m_hInst);

	SetBkMode(m_hMemDC[0], TRANSPARENT);
	SetTextColor(m_hMemDC[0], RGB(255, 255, 255));

	m_pCursur = NULL;

	m_pResource = new ResManager;
	m_pResource->Init(hdc, m_hInst);

	MainMenu();
	InitTile();
	InitTank(PLAYER); 
	ReleaseDC(hWnd, hdc);
} 

void GameManager::ResetGame()
{
	m_fTitlePosY = 600;
	m_pTitleLogo->SetPos(100, 400);
	m_pPlayer->Init(3 * 33, 13 * 25, PLAYER);

	for (auto i = m_vcEnemy.begin(); i != m_vcEnemy.end(); i++)
	{
		(*i)->Die();
		(*i)->ResetScore();
		(*i)->Reload();
	}

	auto Tile = m_vcTile.begin();
	for (int i = 0; i < m_iTileNum + 1; i++)
	{
		for (int j = 0; j < m_iTileNum; j++)
		{
			(*Tile)->InitPos(i * 33, j * 25);
			if (i == 0 || j == 0 || i == 14 || j == 14 || i == 15)
				(*Tile)->InitBitMap(m_pResource->GetBitMap((RES_TYPE)((int)BLOCK_15)), BLOCK_15);

			Tile++;
		}
	}
	InitLoadTile();

	m_fDelay = 0;
	m_iEnemy = 16;
	m_iFieldEnemy = 0;
	m_fGenerationDelay = 400;
	m_iGameState = GAME_STATE_MAIN;
	m_iLife = 2;
	m_iScore = 0;
}

void GameManager::MainMenu()
{
	m_pTitleLogo = new ShowBitMap;
	m_pTitleLogo->Init(100, 400, m_pResource->GetBitMap((RES_TYPE)((int)TITLE_LOGO)));
}

void GameManager::Render()
{
	HDC hdc = GetDC(m_hWnd);
	TCHAR str[128];

	SetTextColor(m_hMemDC[0], RGB(255, 255, 255));
	if (m_iGameState == GAME_STATE_MAIN)
	{
		BitBlt(m_hMemDC[0], 0, 0, 1024, 768, m_hMemDC[1], 0, 0, SRCCOPY);
		m_fDelay = 0;
		BitBlt(m_hMemDC[0], m_pTitleLogo->GetPoint().x, m_pTitleLogo->GetPoint().y, 1024, 768, m_pTitleLogo->Draw(), 0, 0, SRCCOPY);

		wsprintf(str, TEXT("START"));
		TextOut(m_hMemDC[0], 185, (int)m_fTitlePosY - 40, str, lstrlen(str));
		wsprintf(str, TEXT("MAP MAKER"));
		TextOut(m_hMemDC[0], 185, (int)m_fTitlePosY, str, lstrlen(str));

		if (m_pTitleLogo->GetPoint().y > 50)
		{
			m_pTitleLogo->SetPos(m_pTitleLogo->GetPoint().x, m_pTitleLogo->GetPoint().y - 70 * m_fElapseTime);
			m_fTitlePosY = m_fTitlePosY - 130 * m_fElapseTime;
			if (GetKeyState(VK_SPACE) & 0x8000)
			{
				m_pTitleLogo->SetPos(m_pTitleLogo->GetPoint().x, 51);
				m_fTitlePosY = 250;
				return;
			}
		}
		else
		{
			if (m_pCursur == NULL)
			{
				m_pCursur = new Cursur;
				m_pCursur->Init(185 - TANK_SIZE_X, (int)m_fTitlePosY - 45);
				m_pCursur->InitBitMap(m_pResource->GetBitMap((RES_TYPE)((int)TANK_RIGHT_00)));
			}
			else
			{
				if (GetKeyState(VK_UP) & 0x8000)
					m_pCursur->TankCursur(VK_UP);
				else if (GetKeyState(VK_DOWN) & 0x8000)
					m_pCursur->TankCursur(VK_DOWN);
				else if (GetKeyState(VK_SPACE) & 0x8000)
				{
					if (m_pCursur->GetState() == VK_UP)
						m_iGameState = GAME_STATE_WAIT;
					else if (m_pCursur->GetState() == VK_DOWN)
					{
						m_iGameState = GAME_STATE_MAPTOOL;
						InvalidateRect(m_hWnd, NULL, FALSE);
					}
				}
				else
				
				TransparentBlt(m_hMemDC[0], m_pCursur->GetPoint().x, m_pCursur->GetPoint().y, m_pCursur->ReturnHbitmap().x, m_pCursur->ReturnHbitmap().y, m_pCursur->Draw(), 0, 0, m_pCursur->ReturnHbitmap().x, m_pCursur->ReturnHbitmap().y, RGB(255, 0, 255));
			}

			if(m_pCursur->GetState() != VK_DOWN)
				m_pCursur->Init(185 - TANK_SIZE_X, (int)m_fTitlePosY - 45);
		}

		BitBlt(hdc, 0, 0, 1024, 768, m_hMemDC[0], 0, 0, SRCCOPY);
		ReleaseDC(m_hWnd, hdc);

		return;
	}
	else if (m_iGameState == GAME_STATE_WAIT)
	{
		BitBlt(m_hMemDC[0], 0, 0, 1024, 768, m_hMemDC[1], 0, 0, SRCCOPY);
		m_fDelay = m_fDelay + 70 * m_fElapseTime;

		if (m_fDelay > 150)
		{
			m_iGameState = GAME_STATE_RUN;
			m_fDelay = 0;
		}

		wsprintf(str, TEXT("STAGE 01"));
		TextOut(m_hMemDC[0], m_iTileNum * TILE_SIZE_X / 2 - 50, m_iTileNum * TILE_SIZE_Y / 2, str, lstrlen(str));

		BitBlt(hdc, 0, 0, 1024, 768, m_hMemDC[0], 0, 0, SRCCOPY);
		ReleaseDC(m_hWnd, hdc);
	}
	else if (m_iGameState == GAME_STATE_SHOWSCORE)
	{
		BitBlt(m_hMemDC[0], 0, 0, 1024, 768, m_hMemDC[1], 0, 0, SRCCOPY);
		m_fDelay = m_fDelay + 70 * m_fElapseTime;
		if (m_fDelay > 400)
		{
			m_iGameState = GAME_STATE_END;
			m_fDelay = 0;
		}

		wsprintf(str, TEXT("STAGE 01"));
		TextOut(m_hMemDC[0], 7 * TILE_SIZE_X, 3 * TILE_SIZE_Y, str, lstrlen(str));

		wsprintf(str, TEXT("1P SCORE"));
		TextOut(m_hMemDC[0], 3 * TILE_SIZE_X, 5 * TILE_SIZE_Y, str, lstrlen(str));
		wsprintf(str, TEXT("%d"), m_iScore * 100);
		TextOut(m_hMemDC[0], 3 * TILE_SIZE_X, (5 * TILE_SIZE_Y) + 15, str, lstrlen(str));
		
		wsprintf(str, TEXT("%d PTS    |"), m_iScore * 100);
		TextOut(m_hMemDC[0], m_pEnemyIcon->GetPoint().x - 100, m_pEnemyIcon->GetPoint().y, str, lstrlen(str));
		wsprintf(str, TEXT("%d ←"), m_iScore);
		TextOut(m_hMemDC[0], m_pEnemyIcon->GetPoint().x - 10, m_pEnemyIcon->GetPoint().y, str, lstrlen(str));
		TransparentBlt(m_hMemDC[0], m_pEnemyIcon->GetPoint().x + 20, m_pEnemyIcon->GetPoint().y - 5, TANK_SIZE_X, TANK_SIZE_Y, m_pEnemyIcon->Draw(), 0, 0, TANK_SIZE_X, TANK_SIZE_Y, RGB(255, 0, 255));
	
		BitBlt(hdc, 0, 0, 1024, 768, m_hMemDC[0], 0, 0, SRCCOPY);
		ReleaseDC(m_hWnd, hdc);
	}
	else if (m_iGameState == GAME_STATE_GAMEOVER)
	{
		SetTextColor(m_hMemDC[0], RGB(255, 0, 0));

		m_fDelay = m_fDelay + 70 * m_fElapseTime;

		if (m_fDelay > 200)
		{
			m_iGameState = GAME_STATE_SHOWSCORE;
			m_fDelay = 0;
		}

		wsprintf(str, TEXT("GAME OVER"));
		TextOut(m_hMemDC[0], 6 * TILE_SIZE_X, 190, str, lstrlen(str));
		BitBlt(hdc, 0, 0, 1024, 768, m_hMemDC[0], 0, 0, SRCCOPY);

		ReleaseDC(m_hWnd, hdc);
	}

	if (m_iGameState != GAME_STATE_RUN)
		return;

	BitBlt(m_hMemDC[0], 0, 0, 1024, 768, m_hMemDC[1], 0, 0, SRCCOPY);
	for (auto i = m_vcTile.begin(); i != m_vcTile.end(); i++)
	{
		if ((*i)->GetCheckGetBitMap() || (*i)->GetState())
			TransparentBlt(m_hMemDC[0], (*i)->GetPoint().x, (*i)->GetPoint().y, (*i)->ReturnSize().x, (*i)->ReturnSize().y, (*i)->Draw(), 0, 0, (*i)->ReturnSize().x, (*i)->ReturnSize().y, RGB(255, 0, 255));
	}

	for (auto i = m_vcEnemy.begin(); i != m_vcEnemy.end(); i++)
	{
		if ((*i)->GetLife())
			TransparentBlt(m_hMemDC[0], (*i)->GetPoint().x, (*i)->GetPoint().y, (*i)->ReturnSize().x, (*i)->ReturnSize().y, (*i)->Draw(), 0, 0, (*i)->ReturnSize().x, (*i)->ReturnSize().y, RGB(255, 0, 255));

		if ((*i)->GetLifeMissile())
			TransparentBlt(m_hMemDC[0], (*i)->GetMissile()->GetPoint().x, (*i)->GetMissile()->GetPoint().y, (*i)->GetMissile()->ReturnHbitmap().x, (*i)->GetMissile()->ReturnHbitmap().y, (*i)->GetMissile()->Draw(), 0, 0, (*i)->GetMissile()->ReturnHbitmap().x, (*i)->GetMissile()->ReturnHbitmap().y, RGB(255, 0, 255));

		if ((*i)->GetShowScore())
		{
			wsprintf(str, TEXT("100"));
			TextOut(m_hMemDC[0], (*i)->GetPoint().x + (TILE_SIZE_X  / 2) + 5, (*i)->GetPoint().y + (TILE_SIZE_Y / 2) + 5, str, lstrlen(str));
		}
	}

	TransparentBlt(m_hMemDC[0], m_pPlayer->GetPoint().x, m_pPlayer->GetPoint().y, m_pPlayer->ReturnSize().x, m_pPlayer->ReturnSize().y, m_pPlayer->Draw(), 0, 0, m_pPlayer->ReturnSize().x, m_pPlayer->ReturnSize().y, RGB(255, 0, 255));
	if (m_pPlayer->GetLifeMissile())
		TransparentBlt(m_hMemDC[0], m_pPlayer->GetMissile()->GetPoint().x, m_pPlayer->GetMissile()->GetPoint().y, m_pPlayer->GetMissile()->ReturnHbitmap().x, m_pPlayer->GetMissile()->ReturnHbitmap().y, m_pPlayer->GetMissile()->Draw(), 0, 0, m_pPlayer->GetMissile()->ReturnHbitmap().x, m_pPlayer->GetMissile()->ReturnHbitmap().y, RGB(255, 0, 255));
	
	if (m_pPlayer->GetShield() && m_pPlayer->GetSpawnState())
		TransparentBlt(m_hMemDC[0], m_pPlayer->GetPoint().x, m_pPlayer->GetPoint().y, m_pPlayer->ReturnSizeShield().x, m_pPlayer->ReturnSizeShield().y, m_pPlayer->DrawShield(), 0, 0, m_pPlayer->ReturnSizeShield().x, m_pPlayer->ReturnSizeShield().y, RGB(255, 0, 255));

	int iLen = 0;
	for (int i = 0; i < m_iEnemy; i++)
	{
		if (i % 2 == 0)
			iLen++;
		TransparentBlt(m_hMemDC[0], m_pLife->GetPoint().x + ((i % 2) * 14), m_pLife->GetPoint().y + (iLen * 12), 14, 12, m_pLife->Draw(), 0, 0, 14, 12, RGB(255, 0, 255));
	}
	
	TransparentBlt(m_hMemDC[0], m_pPlayerIcon->GetPoint().x, m_pPlayerIcon->GetPoint().y, 14, 12, m_pPlayerIcon->Draw(), 0, 0, 14, 12, RGB(255, 0, 255));
	SetTextColor(m_hMemDC[0], RGB(0, 0, 0));
	wsprintf(str, TEXT("%d"), m_iLife);
	TextOut(m_hMemDC[0], m_pPlayerIcon->GetPoint().x + 14, m_pPlayerIcon->GetPoint().y - 2, str, lstrlen(str));

	BitBlt(hdc, 0, 0, 1024, 768, m_hMemDC[0], 0, 0, SRCCOPY);

	ReleaseDC(m_hWnd, hdc);
}
void GameManager::Release()
{
	m_pResource->Release();
	m_pPlayer->Release();
	for (auto i = m_vcTile.begin(); i != m_vcTile.end(); i++)
	{
		SAFE_DELETE((*i));
	}
	for (auto i = m_vcEnemy.begin(); i != m_vcEnemy.end(); i++)
	{
		(*i)->Release();
		SAFE_DELETE((*i));
	}

	SAFE_DELETE(m_pResource);
	SAFE_DELETE(m_pPlayer);
	SAFE_DELETE(m_pCursur);
	SAFE_DELETE(m_pTitleLogo);
	SAFE_DELETE(m_pLife);
	SAFE_DELETE(m_pPlayerIcon);
	SAFE_DELETE(m_pEnemyIcon);

	delete m_Instance;
}

void GameManager::Updata()
{
	m_dwCurTime = GetTickCount();

	m_fElapseTime = (m_dwCurTime - m_dwLastTime) / 1000.0f;
	m_dwLastTime = m_dwCurTime;

	if (m_iGameState == GAME_STATE_MAPTOOL)
		return;

	if (m_iGameState == GAME_STATE_END)
	{
		ResetGame();
		return;
	}

	Input();
	GenerationEnemy();
	Action();
	Motion();
	MoveCrash();
	CheckEnd();
	Render();
}


void GameManager::MapTool()
{
	
}


void GameManager::InitTile()
{
	m_pLife = new ShowBitMap;
	m_pLife->Init(m_iTileNum * TILE_SIZE_X - 15, 2 * TILE_SIZE_Y, m_pResource->GetBitMap((RES_TYPE)((int)ENEMY_ICON)));
	m_pPlayerIcon = new ShowBitMap;
	m_pPlayerIcon->Init(m_iTileNum * TILE_SIZE_X - 15, 8 * TILE_SIZE_Y, m_pResource->GetBitMap((RES_TYPE)((int)PLAYER_ICON)));
	m_pEnemyIcon = new ShowBitMap;
	m_pEnemyIcon->Init(7 * TILE_SIZE_X, 8 * TILE_SIZE_Y, m_pResource->GetBitMap((RES_TYPE)((int)E_RIGHT_00)));
	
	for (int i = 0; i < m_iTileNum + 1; i++)
	{
		for (int j = 0; j < m_iTileNum; j++)
		{
			Tile* pTile = new Tile;
			pTile->InitPos(i * 33, j * 25);
			if (i == 0 || j == 0 || i == 14 || j == 14 || i == 15)
				pTile->InitBitMap(m_pResource->GetBitMap((RES_TYPE)((int)BLOCK_15)), BLOCK_15);

			for (int i = 0; i < 5; i++)
			{
				pTile->InitExplosion(m_pResource->GetBitMap((RES_TYPE)((int)EXPLOSION_00 + i)), i);
			}

			m_vcTile.push_back(pTile);
		}
	}

	InitLoadTile();
}
void GameManager::InitLoadTile()
{
	HANDLE hFile = CreateFile("save_00", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	for (auto i = m_vcTile.begin(); i != m_vcTile.end(); i++)
	{
		int iType;
		DWORD writeB;
		ReadFile(hFile, &iType, sizeof(int), &writeB, NULL);

		if (iType >= BLOCK_00 && iType <= BLOCK_15)
			(*i)->InitBitMap(m_pResource->GetBitMap((RES_TYPE)((int)iType)), iType);
	}

	CloseHandle(hFile);

	InvalidateRect(m_hWnd, NULL, FALSE);
}

void GameManager::InitTank(int Team)
{
	int iBitMap;
	Tank* pTank;
	if (Team == ENEMY)
	{
		pTank = new Enemy;
		iBitMap = E_DOWN_00;
		int Random = rand() % 13 + 1;
		pTank->Init(Random * TILE_SIZE_X, TILE_SIZE_Y, ENEMY);
	}
	else if (Team == PLAYER)
	{
		pTank = new Player;
		iBitMap = TANK_DOWN_00;
		pTank->Init(3 * 33, 13 * 25, PLAYER);
	}

	int iCount = 0;
	for (int i = iBitMap; i <= iBitMap + 8; i++)
	{
		pTank->InitBitMap(m_pResource->GetBitMap((RES_TYPE)((int)i)), iCount);
		iCount++;
	}
	for (int i = 0; i < 5; i++)
	{
		pTank->InitExplosion(m_pResource->GetBitMap((RES_TYPE)((int)EXPLOSION_00 + i)), i);
	}
	for (int i = 0; i < 4; i++)
	{
		pTank->InitMissile(m_pResource->GetBitMap((RES_TYPE)((int)MISSILE_00 + i)), i);
		pTank->InitSpawn(m_pResource->GetBitMap((RES_TYPE)((int)SPAWN_00 + i)), i);
	}
	if (Team == PLAYER)
	{
		for (int i = 0; i < 2; i++)
		{
				pTank->InitShield(m_pResource->GetBitMap((RES_TYPE)((int)SHIELD_00 + i)), i);
		}
	}

	if (Team == ENEMY)
		m_vcEnemy.push_back(pTank);
	else if (Team == PLAYER)
		m_pPlayer = pTank;
}

void GameManager::Input()
{	
	m_iKeyState = m_pPlayer->MoveTo(m_fElapseTime);

	if (GetKeyState(VK_SPACE) & 0x8000)
		m_pPlayer->Attack();

	MoveControl();
}

void GameManager::MoveControl()
{
	if (m_iKeyState == NULL)
		return;

	RECT rcResult;
	RECT rcOne = { m_pPlayer->GetPoint().x, m_pPlayer->GetPoint().y, m_pPlayer->GetPoint().x + TANK_SIZE_X , m_pPlayer->GetPoint().y + TANK_SIZE_Y	};
	for (auto i = m_vcTile.begin(); i != m_vcTile.end(); i++)
	{	
		if ((*i)->CheckHitCount(m_iKeyState))
			return;

		RECT rcTwo = (*i)->GetSize();
		
		if ((*i)->GetCheckGetBitMap())
		{
			if (IntersectRect(&rcResult, &rcOne, &rcTwo))
			{
				int iLen = 0;
				if (m_iKeyState == VK_UP)
				{
					if ((*i)->GetSize().left + 30 > m_pPlayer->GetPoint().x && (*i)->GetSize().left - 30 < m_pPlayer->GetPoint().x)
						return;
					

					if (m_pPlayer->GetPoint().x > (*i)->GetPoint().x)
						m_pPlayer->SetPos((*i)->GetPoint().x + TILE_SIZE_X, 0);
					else if (m_pPlayer->GetPoint().x < (*i)->GetPoint().x)
						m_pPlayer->SetPos((*i)->GetPoint().x - TILE_SIZE_X, 0);
				}
				else if (m_iKeyState == VK_DOWN)
				{
					if ((*i)->GetSize().left + 30 > m_pPlayer->GetPoint().x && (*i)->GetSize().left - 30 < m_pPlayer->GetPoint().x)
						return;

					if (m_pPlayer->GetPoint().x >(*i)->GetPoint().x)
						m_pPlayer->SetPos((*i)->GetPoint().x + TILE_SIZE_X, 0);
					else if (m_pPlayer->GetPoint().x < (*i)->GetPoint().x)
						m_pPlayer->SetPos((*i)->GetPoint().x - TILE_SIZE_X, 0);
				}
				else if (m_iKeyState == VK_RIGHT)
				{
					if ((*i)->GetSize().top + 20 > m_pPlayer->GetPoint().y && (*i)->GetSize().top - 20 < m_pPlayer->GetPoint().y)
						return;

					if (m_pPlayer->GetPoint().y > (*i)->GetPoint().y)
						m_pPlayer->SetPos(0, (*i)->GetPoint().y + TILE_SIZE_Y);
					else if (m_pPlayer->GetPoint().y < (*i)->GetPoint().y)
						m_pPlayer->SetPos(0, (*i)->GetPoint().y - TILE_SIZE_Y);

				
				}
				else if (m_iKeyState == VK_LEFT)
				{
					if ((*i)->GetSize().top + 20 > m_pPlayer->GetPoint().y && (*i)->GetSize().top - 20 < m_pPlayer->GetPoint().y)
						return;

					if (m_pPlayer->GetPoint().y > (*i)->GetPoint().y)
						m_pPlayer->SetPos(0, (*i)->GetPoint().y + TILE_SIZE_Y);
					else if (m_pPlayer->GetPoint().y < (*i)->GetPoint().y)
						m_pPlayer->SetPos(0, (*i)->GetPoint().y - TILE_SIZE_Y);
				}		
				return;
			}
		}
	}

}

void GameManager::Action()
{	
	if (m_iGameState != GAME_STATE_RUN)
		return;

	m_pPlayer->GetMissile()->Move(m_fElapseTime);

	for (auto i = m_vcEnemy.begin(); i != m_vcEnemy.end(); i++)
	{
		if ((*i)->GetLife())
		{
			(*i)->MoveTo(m_fElapseTime); // 움직임
			(*i)->AttackTo(m_fElapseTime); // 공격
		}

		(*i)->ScoreDelay(m_fElapseTime);
		(*i)->GetMissile()->Move(m_fElapseTime);
	}
}
void GameManager::Motion()
{
	if (m_iGameState != GAME_STATE_RUN)
		return;

	m_pPlayer->ShieldMotion(m_fElapseTime);
	m_pPlayer->SpawnMotion(m_fElapseTime);
	for (auto i = m_vcEnemy.begin(); i != m_vcEnemy.end(); i++)
	{
		(*i)->SpawnMotion(m_fElapseTime);
	}
}

void GameManager::MoveCrash()
{
	if (m_iGameState != GAME_STATE_RUN)
		return;

	MoveCrashTile(m_pPlayer); 
	AttackCrash(m_pPlayer);
	for (auto i = m_vcEnemy.begin(); i != m_vcEnemy.end(); i++)
	{
		MoveCrashTile((*i));
		AttackCrash((*i));
	}

	MoveCrashEnemy();
}
void GameManager::MoveCrashTile(Tank* pTank)
{
	if (pTank->CheckState())
		return;

	RECT rcResult;
	RECT rcOne = { pTank->GetPoint().x, pTank->GetPoint().y, pTank->GetPoint().x + 31 , pTank->GetPoint().y + 24 };
	for (auto i = m_vcTile.begin(); i != m_vcTile.end(); i++)
	{
		RECT rcTwo = (*i)->GetSize();
		if ((*i)->IsTileState())
		{
			if (IntersectRect(&rcResult, &rcOne, &rcTwo))
			{
				CrashMove(pTank);

				if (pTank->GetTeam() == ENEMY)
					pTank->ChangeWay(TANK_STATE_DOWN_00, m_fElapseTime);
			}
		}
	}

}
void GameManager::MoveCrashEnemy()
{
	RECT rcResult;
	for (auto i = m_vcEnemy.begin(); i != m_vcEnemy.end(); i++)
	{
		for (auto j = m_vcEnemy.begin(); j != m_vcEnemy.end(); j++)
		{
			if ((*i) != (*j) && (*i)->GetLife() && (*j)->GetLife())
			{
				RECT rcOne = { (*i)->GetPoint().x, (*i)->GetPoint().y, (*i)->GetPoint().x + TANK_SIZE_X , (*i)->GetPoint().y + TANK_SIZE_Y };
				RECT rcTwo = { (*j)->GetPoint().x, (*j)->GetPoint().y, (*j)->GetPoint().x + TANK_SIZE_X, (*j)->GetPoint().y + TANK_SIZE_Y};
				if (IntersectRect(&rcResult, &rcOne, &rcTwo))
				{
					if((*i)->GetSpawnState())
						CrashMove((*i));
					if ((*j)->GetSpawnState())
						CrashMove((*j));
					(*i)->EnemyCrash(); // 충돌처리 방향
					(*j)->EnemyCrash();
				}
			}
		}
	}
}
void GameManager::CrashMove(Tank* pTank)
{
	if (pTank->GetState() == TANK_STATE_LEFT_00 || pTank->GetState() == TANK_STATE_LEFT_01)
		pTank->MoveBack(m_fElapseTime, VK_RIGHT);
	else if (pTank->GetState() == TANK_STATE_RIGHT_00 || pTank->GetState() == TANK_STATE_RIGHT_01)
		pTank->MoveBack(m_fElapseTime, VK_LEFT);
	else if (pTank->GetState() == TANK_STATE_UP_00 || pTank->GetState() == TANK_STATE_UP_01)
		pTank->MoveBack(m_fElapseTime, VK_DOWN);
	else if (pTank->GetState() == TANK_STATE_DOWN_00 || pTank->GetState() == TANK_STATE_DOWN_01)
		pTank->MoveBack(m_fElapseTime, VK_UP);
}

void GameManager::GenerationEnemy()
{
	if (m_iGameState != GAME_STATE_RUN)
		return;
	if (m_iFieldEnemy == 4)
		return;
	if (m_iEnemy == 0)
		return;

	m_fGenerationDelay = m_fGenerationDelay + 70 * m_fElapseTime;

	if (m_fGenerationDelay > 300)
	{
		m_iEnemy--;
		m_iFieldEnemy++;
		m_fGenerationDelay = 0;
		InitTank(ENEMY);
	}
}

void GameManager::AttackCrash(Tank* pTank) 
{
	CheckReload(pTank);
	CheckScroeUpdata(pTank);
	AttackCheckTile(pTank);
	AttackCheckTank();
	AttackCheckMissile();
}


void GameManager::AttackCheckTile(Tank* pTank) // 충돌처리
{
	if (pTank->GetMissile()->GetCrashState())
		return;

	RECT rcResult;
	for (auto i = m_vcTile.begin(); i != m_vcTile.end(); i++)
	{
		if ((*i)->IsTileState())
		{
			RECT rcOne = (*i)->GetSize();
			RECT rcTwo = { pTank->GetMissile()->GetPoint().x, pTank->GetMissile()->GetPoint().y, pTank->GetMissile()->GetPoint().x + 8, pTank->GetMissile()->GetPoint().y + 5 };
			if (IntersectRect(&rcResult, &rcOne, &rcTwo))
			{
				(*i)->BreakTile(pTank->GetMissile()->GetWay());
				pTank->GetMissile()->Explosion();

				if ((*i)->GetType() == BLOCK_13)
					(*i)->Explosion();

				return;
			}
		}
	}
}
void GameManager::AttackCheckTank()
{
	RECT rcResult;

	for (auto i = m_vcEnemy.begin(); i != m_vcEnemy.end(); i++)
	{
		RECT rcOne = { m_pPlayer->GetPoint().x, m_pPlayer->GetPoint().y, m_pPlayer->GetPoint().x + TANK_SIZE_X, m_pPlayer->GetPoint().y + TANK_SIZE_Y };
		RECT rcTwo = { (*i)->GetMissile()->GetPoint().x, (*i)->GetMissile()->GetPoint().y, (*i)->GetMissile()->GetPoint().x + 8, (*i)->GetMissile()->GetPoint().y + 5 };
		if (IntersectRect(&rcResult, &rcOne, &rcTwo))
		{
			if ((*i)->GetLifeMissile() && m_pPlayer->GetExplosion() == false && m_pPlayer->GetMissile()->GetCrashState() == false)
			{
				if (m_pPlayer->GetShield() == false && m_pPlayer->GetSpawnState())
					m_pPlayer->Explosion();

				(*i)->GetMissile()->Explosion();
			}
		}
	}

	for (auto i = m_vcEnemy.begin(); i != m_vcEnemy.end(); i++)
	{
		RECT rcOne = { m_pPlayer->GetMissile()->GetPoint().x, m_pPlayer->GetMissile()->GetPoint().y, m_pPlayer->GetMissile()->GetPoint().x + 8, m_pPlayer->GetMissile()->GetPoint().y + 5 };
		RECT rcTwo = { (*i)->GetPoint().x, (*i)->GetPoint().y, (*i)->GetPoint().x + TANK_SIZE_X, (*i)->GetPoint().y + TANK_SIZE_Y };
		if (IntersectRect(&rcResult, &rcOne, &rcTwo))
		{
			if (m_pPlayer->GetLifeMissile() && (*i)->GetExplosion() == false && m_pPlayer->GetMissile()->GetCrashState() == false)
			{
				m_pPlayer->GetMissile()->Explosion();

				if ((*i)->GetSpawnState())
					(*i)->Explosion();
			}
		}

	}
}
void GameManager::AttackCheckMissile()
{
	RECT rcResult; 
	for (auto i = m_vcEnemy.begin(); i != m_vcEnemy.end(); i++)
	{
		if (m_pPlayer->GetLifeMissile() && (*i)->GetLifeMissile())
		{
			RECT rcOne = { m_pPlayer->GetMissile()->GetPoint().x, m_pPlayer->GetMissile()->GetPoint().y, m_pPlayer->GetMissile()->GetPoint().x + 8, m_pPlayer->GetMissile()->GetPoint().y + 5 };
			RECT rcTwo = { (*i)->GetMissile()->GetPoint().x, (*i)->GetMissile()->GetPoint().y, (*i)->GetMissile()->GetPoint().x + 8, (*i)->GetMissile()->GetPoint().y + 5 };
			if (IntersectRect(&rcResult, &rcOne, &rcTwo))
			{
				m_pPlayer->GetMissile()->Explosion();
				(*i)->GetMissile()->Explosion();
			}
		}
	}
}

void GameManager::CheckEnd()
{
	if (m_iGameState != GAME_STATE_RUN)
		return;

	if (m_iEnemy == 0 && m_iFieldEnemy == 0)
		m_iGameState = GAME_STATE_SHOWSCORE;
	if (m_iLife == -1)
		m_iGameState = GAME_STATE_GAMEOVER;

	for (auto i = m_vcTile.begin(); i != m_vcTile.end(); i++)
	{
		(*i)->ExplosionMotion(m_fElapseTime);

		if ((*i)->GetType() == BLOCK_14)
			m_iGameState = GAME_STATE_GAMEOVER;
		if ((*i)->GetExplosionState() == TANK_STATE_EXPLOSION_04)
			(*i)->InitBitMap(m_pResource->GetBitMap((RES_TYPE)((int)BLOCK_14)), BLOCK_14);
	}
}
void GameManager::CheckReload(Tank* pTank)// 미사일 발사 확인
{
	if (pTank->GetLifeMissile())
	{
		if (pTank->GetMissile()->ExplosionMotion(m_fElapseTime))
		{
			pTank->Reload();
			return;
		}
	}
}
void GameManager::CheckScroeUpdata(Tank* pTank)
{
	if (pTank->GetExplosion() == false)
		return;

	if (pTank->ExplosionMotion(m_fElapseTime))
	{
		if (pTank->GetTeam() == ENEMY)
		{
			pTank->Die();
			m_iFieldEnemy--;
			m_iScore++;
		}
		else if (pTank->GetTeam() == PLAYER)
		{
			pTank->ReSpawn();
			m_iLife--;
		}
	}
}
