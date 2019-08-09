#pragma once
#include <Windows.h>
#include <vector>
#include <time.h>

using namespace std;
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }

enum GAME_STATE
{
	GAME_STATE_MAIN,
	GAME_STATE_WAIT,
	GAME_STATE_RUN,
	GAME_STATE_MAPTOOL,
	GAME_STATE_GAMEOVER,
	GAME_STATE_SHOWSCORE,
	GAME_STATE_END,
};

class ResManager;
class Tile;
class Tank;
class Cursur;
class Player;
class Enemy;
class ShowBitMap;
class GameManager
{
private:
	static GameManager* m_Instance;

	HWND m_hWnd;
	HINSTANCE m_hInst;

	DWORD m_dwLastTime;
	DWORD m_dwCurTime;
	float m_fElapseTime;

	HDC		m_hMemDC[2];
	HBITMAP	m_hBitMap[2];
	HBITMAP	m_hOldBitMap[2];
	
	vector <Tile*> m_vcTile;
	vector <Tank*> m_vcEnemy;

	ResManager* m_pResource;
	Tank* m_pPlayer;
	Cursur* m_pCursur;
	ShowBitMap* m_pTitleLogo;
	ShowBitMap* m_pLife;
	ShowBitMap* m_pPlayerIcon;
	ShowBitMap* m_pEnemyIcon;

	float m_fTitlePosY;
	float m_fDelay;
	float m_fGenerationDelay;
	int m_iGameState;
	int m_iEnemy;
	int m_iLife;
	int m_iFieldEnemy;
	int m_iScore;

	int m_iTileNum;
	int m_iKeyState;
	GameManager();
	~GameManager();
public:
	static GameManager* GetInstance();

	void Init(HWND hWnd, HINSTANCE g_hInst);
	void Render();
	void Release();
	void Updata();

	void Input();
	
	void MoveControl();
	void MapTool();

	void MainMenu();

	
	//√ ±‚»≠
	void InitTile();
	void InitLoadTile();
	void ResetGame();

	void Action();
	void Motion();
	void GenerationEnemy();
	void MoveCrash();
	void MoveCrashEnemy();
	void CheckEnd();
	void AttackCheckTank();
	void AttackCheckMissile();
	void MoveCrashTile(Tank* pTank);
	void CrashMove(Tank* pTank);
	void AttackCheckTile(Tank* pTank);
	void CheckScroeUpdata(Tank* pTank);
	void AttackCrash(Tank* pTank);
	void InitTank(int Team);
	void CheckReload(Tank* pTank);
	
	virtual inline void ChangeState()
	{
		ResetGame();
	}
	virtual inline int GetGameState()
	{
		return m_iGameState;
	}
};

