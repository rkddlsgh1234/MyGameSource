#include "MapToolManager.h"
#include "ResManager.h"
#include "Tile.h"
#include "resource.h"
#include "GameManager.h"

MapToolManager::MapToolManager()
{
}


MapToolManager::~MapToolManager()
{
}

MapToolManager* MapToolManager::m_Instance = NULL;

MapToolManager* MapToolManager::GetInstance()
{
	if (m_Instance == NULL)
	{
		m_Instance = new MapToolManager;
	}

	return m_Instance;
}

void MapToolManager::Init(HWND Wnd, HINSTANCE hInst)
{
	m_bCheckDrawMode = true;
	m_iSelectState = BLOCK_00;
	hWnd = Wnd;
	HDC hdc = GetDC(hWnd);
	CheckDlgButton(hWnd, BLOCK_00, TRUE);

	m_pResource = new ResManager;
	m_pResource->Init(hdc, hInst);

	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			Tile* pTile = new Tile;
			pTile->InitPos(i * 33, j * 25);
			if (i == 0 || j == 0 || i == 14 || j == 14)
				pTile->InitBitMap(m_pResource->GetBitMap((RES_TYPE)((int)BLOCK_15)), BLOCK_15);
			m_vcTile.push_back(pTile);
		}
	}

	TCHAR str[128];
	for (int i = BLOCK_00; i <= BLOCK_14; i++)
	{
		Tile* pTile = new Tile;
		if(i < BLOCK_14 / 2)
			pTile->InitPos(520, 10 + (40 * i));
		else
			pTile->InitPos(520 + TILE_SIZE_X + 10, 10 + (40 * (i - BLOCK_14 / 2)));

		pTile->InitBitMap(m_pResource->GetBitMap((RES_TYPE)((int)i)), i);
		m_vcMapTool.push_back(pTile);
	}
	CreateWindow(TEXT("button"), TEXT("저장"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, 400, 100, 30, hWnd, (HMENU)50, hInst, NULL);
	CreateWindow(TEXT("button"), TEXT("불러오기"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 100, 400, 100, 30, hWnd, (HMENU)51, hInst, NULL);

	m_hMemDC[0] = CreateCompatibleDC(hdc);
	m_hBitMap[0] = CreateCompatibleBitmap(hdc, 600, 440);
	m_hOldBitMap[0] = (HBITMAP)SelectObject(m_hMemDC[0], m_hBitMap[0]);

	m_hMemDC[1] = CreateCompatibleDC(m_hMemDC[0]);
	m_hBitMap[1] = CreateCompatibleBitmap(hdc, 600, 440);
	m_hOldBitMap[1] = (HBITMAP)SelectObject(m_hMemDC[1], m_hBitMap[1]);
}

void MapToolManager::Draw()
{
	if (GameManager::GetInstance()->GetGameState() != GAME_STATE_MAPTOOL)
		return;

	HDC hdc = GetDC(hWnd);

	BitBlt(m_hMemDC[0], 0, 0, 1280, 720, m_hMemDC[1], 0, 0, SRCCOPY);
	for (auto i = m_vcMapTool.begin(); i != m_vcMapTool.end(); i++)
	{
		TransparentBlt(m_hMemDC[0], (*i)->GetPoint().x, (*i)->GetPoint().y, 33, 25, (*i)->Draw(), 0, 0, 33, 25, RGB(255, 0, 255));
	}

	for (auto i = m_vcTile.begin(); i != m_vcTile.end(); i++)
	{
		if((*i)->GetCheckGetBitMap())
			TransparentBlt(m_hMemDC[0], (*i)->GetPoint().x, (*i)->GetPoint().y, 33, 25, (*i)->Draw(), 0, 0, 33, 25, RGB(255, 0, 255));
	}

	BitBlt(hdc, 0, 0, 1280, 720, m_hMemDC[0], 0, 0, SRCCOPY);
	ReleaseDC(hWnd, hdc);
}

void MapToolManager::Updata()
{

}

void MapToolManager::Release()
{
	for (auto i = m_vcTile.begin(); i != m_vcTile.end(); i++)
	{
		SAFE_DELETE(*i);
	}
	for (auto i = m_vcLoad.begin(); i != m_vcLoad.end(); i++)
	{
		SAFE_DELETE(*i);
	}
	for (auto i = m_vcMapTool.begin(); i != m_vcMapTool.end(); i++)
	{
		SAFE_DELETE(*i);
	}
	m_pResource->Release();
	SAFE_DELETE(m_pResource);

	delete m_Instance;
}

void MapToolManager::DrawTile(int x, int y)
{
	if (m_bCheckDrawMode != true)
		return;

	for (auto i = m_vcTile.begin(); i != m_vcTile.end(); i++)
	{
		(*i)->SelectDraw(x, y, m_pResource->GetBitMap((RES_TYPE)((int)m_iSelectState)), m_iSelectState);
	}
}
void MapToolManager::ReMoveTile(int x, int y)
{
	if (m_bCheckDrawMode != true)
		return;

	for (auto i = m_vcTile.begin(); i != m_vcTile.end(); i++)
	{
		(*i)->Remove(x, y);
	}
}

void MapToolManager::SetDrawTool(int x, int y)
{
	for (auto i = m_vcMapTool.begin(); i != m_vcMapTool.end(); i++)
	{
		if ((*i)->SelectTile(x, y))
			m_iSelectState = (*i)->GetType();
	}
}

void MapToolManager::SetDrowMode()
{
	m_bCheckDrawMode = true;
}

void MapToolManager::SaveFile()
{
	OPENFILENAME OFN;
	char str[300];
	char lpstrFile[MAX_PATH] = "save.txt";
	char lpstrPath[MAX_PATH] = "c:\\";

	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner = hWnd;
	OFN.lpstrFilter = "Every File(*.*)\0*.*\0Text File\0*.txt;*.doc\0";
	OFN.lpstrFile = lpstrFile;
	OFN.nMaxFile = MAX_PATH;
	GetCurrentDirectory(MAX_PATH, lpstrPath);
	OFN.lpstrInitialDir = lpstrPath;
	if (GetSaveFileName(&OFN) == 0)
	{
		DWORD err = CommDlgExtendedError();
		return;
	}

	HANDLE hFile = CreateFile(OFN.lpstrFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	for (auto i = m_vcTile.begin(); i != m_vcTile.end(); i++)
	{
		int iType = (*i)->GetType();
		DWORD writeB;
		WriteFile(hFile, &iType, sizeof(int), &writeB, NULL);
	}

	CloseHandle(hFile);
	InvalidateRect(hWnd, NULL, FALSE);
}

void MapToolManager::LoadFile()
{
	HANDLE hFile = CreateFile("save_00", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	
	for (auto i = m_vcTile.begin(); i != m_vcTile.end(); i++)
	{
		(*i)->ResetBitMap();
	}

	for (auto i = m_vcTile.begin(); i != m_vcTile.end(); i++)
	{
		int iType;
		DWORD writeB;
		ReadFile(hFile, &iType, sizeof(int), &writeB, NULL);

		if(iType >= BLOCK_00 && iType <= BLOCK_15)
			(*i)->InitBitMap(m_pResource->GetBitMap((RES_TYPE)((int)iType)), iType);
	}

	CloseHandle(hFile);
	InvalidateRect(hWnd, NULL, FALSE);
}