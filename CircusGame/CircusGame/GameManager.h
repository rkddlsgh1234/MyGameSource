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
	int m_iKeyType; // Ű ���� 
	int m_iObject; // �� Ÿ�� ���� 
	int m_iShowLength; // �׸��� ����
	int m_iEnemyNum; // Enemy ����
	int m_iGenerateDelay; // Enemy ���� ������
	int m_iScore; // ���ھ�
	int m_iBonusScore; // ���ʽ� ���ھ�
	int m_iScoreDelay; // ���ʽ� ���ھ� ���� ������
	int m_iLife; // ������
	int m_iSaveMiter; // Player x ��ġ����
	int m_iGameState;
	int m_iDelay;

	int m_iEndDelay;  // End ������

	bool m_bEnemyControl;
	bool m_bEndLine; // ��¼�

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
	void CrashCheck(); // �浹üũ
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