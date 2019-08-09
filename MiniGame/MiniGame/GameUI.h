#pragma once
#include "BitMap.h"
#include <InputManager.h>
#include "Button.h"
#include "GameMenu.h"

#define CLOCK_POS_X 170
#define CLOCK_POS_Y 760

#define TIMER_NUM_POS_X 200
#define TIMER_NUM_POS_Y 760

#define INFO_MENU_POS_X 70
#define INFO_MENU_POS_Y 100

enum FEVER_STATE
{
	FEVER_STATE_NORMAL,
	FEVER_STATE_FEVER,
	FEVER_STATE_SUPER,
	FEVER_STATE_ULTRA,
};
enum ANIMATION_STATE
{
	ANI_STATE_FRONT,
	ANI_STATE_BACK,
};

class Button;
class GameUI
{
private:
	Engine::BitMap* m_pTimerBar[2];
	Engine::BitMap* m_pFeverBar[4];

	Engine::BitMap* m_pFeverText[3];

	Engine::BitMap* m_pReady[2];

	Engine::BitMap* m_pNumber[10];
	Engine::BitMap* m_pNumberSmall[10];

	Engine::BitMap* m_pComma;
	Engine::BitMap* m_pClockTimer;
	Engine::BitMap* m_pTimeColon;
	Engine::BitMap* m_pFeverBack[4];
	Engine::BitMap* m_pTimeOver[2];

	Button* m_pButton;

	int iCount;
	int m_iDelayIndex;

	float m_fTimeOverPosOne;
	float m_fTimeOverPosTwo;
	float m_fDelay;
	float m_fFeverDelay;
	float m_fFeverTextDelay;
	float m_fFeverPosY;
	float m_fFailDelay;
	float m_fTimeDelay;
	float m_fTimerBarDelay;
	float m_fEndDelay;

	float m_fMotionDelay;

	int m_iAddScore;
	int m_iCombo;
	int m_iTimeCount;

	float Minute;
	int m_iSecond;

	FEVER_STATE m_eFeverState;
	ANIMATION_STATE m_eFeverAniState;
	

	bool m_bCheckFever;
	bool m_bCheckFail;
	bool m_bCheckFeverText;
	int Fever;
public:
	GameUI();
	~GameUI();

	void Init(int DelayIndex);

	bool Updata(float fElapseTime);
	void Draw();
	void DrawWait(int Index);
	void DrawScore(int Score);
	void DrawFever();
	void DrawTime();
	void DrawTimeOver();

	void Release();

	bool CheckButton();
	bool SetSize(float fElapseTime);
	void SetFailFever();

	void MotionFeverEffect(float fElapseTime);
	void MotionFever(float fElapseTime);
	bool TimeOverAnimation(float fElapseTime);

	int ScoreManager(int Score);

	bool Timer(float fElapseTime);


	virtual inline FEVER_STATE GetFeverState()
	{
		return m_eFeverState;
	}
	virtual inline bool GetFeverText()
	{
		return m_bCheckFeverText;
	}

};

