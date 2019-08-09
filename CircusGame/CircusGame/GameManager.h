#pragma once
#include <Windows.h>
#include <vector>

#include "ResManager.h"

using namespace std;

#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SHOW_DISTANCE_ONE 430
#define SHOW_DISTANCE_TWO 530

enum GAME_STATE
{
	GAME_STATE_WAIT,
	GAME_STATE_SHOW_STAGE,
	GAME_STATE_MAIN,
	GAME_STATE_RUN,
	GAME_STATE_END,
};

class ResManager;
class Player;
class BackGround;
class Enemy;
class JarEnemy;
class ShowBitMap;
class Cash;
class MainIntro;
class GameManager
{
private:
	static GameManager* m_Instance;
	int m_iKeyType; // 키 저장 
	int m_iObject; // 백 타일 갯수 
	int m_iShowLength; // 그리는 범위
	int m_iEnemyNum; // Enemy 갯수
	int m_iGenerateDelay; // Enemy 생성 딜레이
	int m_iScore; // 스코어
	int m_iBonusScore; // 보너스 스코어
	int m_iScoreDelay; // 보너스 스코어 감소 딜레이
	int m_iLife; // 라이프
	int m_iSaveMiter; // Player x 위치저장
	int m_iGameState;
	int m_iDelay;

	int m_iEndDelay;  // End 딜레이

	bool m_bEnemyControl;
	bool m_bEndLine; // 결승선

	HDC		m_hMemDC[2];
	HBITMAP	m_hBitMap[2];
	HBITMAP	m_hOldBitMap[2];
	SIZE	m_size;

	ResManager* m_pResource;
	Player* m_pPlayer;
	BackGround* m_pMiter;
	BackGround* m_pEndTile;
	ShowBitMap* m_pScoreBoard;
	ShowBitMap* m_pTitleLogo;
	ShowBitMap* m_pShowStage;
	
	vector <MainIntro*> m_vcMainIntro;
	vector <BackGround*> m_vcBackGround;
	vector <Enemy*> m_vcEnemy;

	GameManager();
	~GameManager();
public:
	static GameManager* GetInstance();

	void Init(HWND hWnd, HINSTANCE hInst, HDC hdc);
	void Draw(HDC hdc);
	void MainDraw();
	void Release();

	void Input(HWND hWnd);

	bool Updata(HWND hWnd);

	void ResetGame();

	void GenerateEnemy();
	void CrashCheck(); // 충돌체크
	void EndLine();
	void MoveScroll();
	void SaveDistance();
	void CrashEnemy();
	void GenerateJarEnemy();
	void MainMenu();

	bool CheckPlayerState();

	virtual inline bool GetEndLine()
	{
		return m_bEndLine;
	}
	virtual inline int GetSaveMiter()
	{
		return m_iSaveMiter;
	}
};