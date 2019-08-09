#include "GameUI.h"



GameUI::GameUI()
{
}


GameUI::~GameUI()
{
}
void GameUI::Init(int DelayIndex)
{
	m_pTimerBar[0] = Engine::ResManager::GetInstance()->InitBitMap("TimerBar_blank.bmp");
	m_pTimerBar[1] = Engine::ResManager::GetInstance()->InitBitMap("TimerBar.bmp");

	m_pFeverBar[0] = Engine::ResManager::GetInstance()->InitBitMap("FeverBar_00.bmp");
	m_pFeverBar[1] = Engine::ResManager::GetInstance()->InitBitMap("FeverBar_01.bmp");
	m_pFeverBar[2] = Engine::ResManager::GetInstance()->InitBitMap("FeverBar_02.bmp");
	m_pFeverBar[3] = Engine::ResManager::GetInstance()->InitBitMap("FeverBar_03.bmp");

	m_pTimeOver[0] = Engine::ResManager::GetInstance()->InitBitMap("TimeOver_00.bmp");
	m_pTimeOver[1] = Engine::ResManager::GetInstance()->InitBitMap("TimeOver_01.bmp");

	m_pFeverText[0] = Engine::ResManager::GetInstance()->InitBitMap("FeverText_00.bmp");
	m_pFeverText[1] = Engine::ResManager::GetInstance()->InitBitMap("FeverText_01.bmp");
	m_pFeverText[2] = Engine::ResManager::GetInstance()->InitBitMap("FeverText_02.bmp");

	m_pReady[0] = Engine::ResManager::GetInstance()->InitBitMap("Ready.bmp");
	m_pReady[1] = Engine::ResManager::GetInstance()->InitBitMap("Go.bmp");

	m_pNumber[0] = Engine::ResManager::GetInstance()->InitBitMap("Number_00.bmp");
	m_pNumber[1] = Engine::ResManager::GetInstance()->InitBitMap("Number_01.bmp");
	m_pNumber[2] = Engine::ResManager::GetInstance()->InitBitMap("Number_02.bmp");
	m_pNumber[3] = Engine::ResManager::GetInstance()->InitBitMap("Number_03.bmp");
	m_pNumber[4] = Engine::ResManager::GetInstance()->InitBitMap("Number_04.bmp");
	m_pNumber[5] = Engine::ResManager::GetInstance()->InitBitMap("Number_05.bmp");
	m_pNumber[6] = Engine::ResManager::GetInstance()->InitBitMap("Number_06.bmp");
	m_pNumber[7] = Engine::ResManager::GetInstance()->InitBitMap("Number_07.bmp");
	m_pNumber[8] = Engine::ResManager::GetInstance()->InitBitMap("Number_08.bmp");
	m_pNumber[9] = Engine::ResManager::GetInstance()->InitBitMap("Number_09.bmp");
	m_pNumberSmall[0] = Engine::ResManager::GetInstance()->InitBitMap("Number_Small_00.bmp");
	m_pNumberSmall[1] = Engine::ResManager::GetInstance()->InitBitMap("Number_Small_01.bmp");
	m_pNumberSmall[2] = Engine::ResManager::GetInstance()->InitBitMap("Number_Small_02.bmp");
	m_pNumberSmall[3] = Engine::ResManager::GetInstance()->InitBitMap("Number_Small_03.bmp");
	m_pNumberSmall[4] = Engine::ResManager::GetInstance()->InitBitMap("Number_Small_04.bmp");
	m_pNumberSmall[5] = Engine::ResManager::GetInstance()->InitBitMap("Number_Small_05.bmp");
	m_pNumberSmall[6] = Engine::ResManager::GetInstance()->InitBitMap("Number_Small_06.bmp");
	m_pNumberSmall[7] = Engine::ResManager::GetInstance()->InitBitMap("Number_Small_07.bmp");
	m_pNumberSmall[8] = Engine::ResManager::GetInstance()->InitBitMap("Number_Small_08.bmp");
	m_pNumberSmall[9] = Engine::ResManager::GetInstance()->InitBitMap("Number_Small_09.bmp");

	m_pComma = Engine::ResManager::GetInstance()->InitBitMap("Comma.bmp");
	m_pClockTimer = Engine::ResManager::GetInstance()->InitBitMap("ClockTimer.bmp");
	m_pTimeColon = Engine::ResManager::GetInstance()->InitBitMap("Time_Colon.bmp");

	m_pFeverBack[0] = Engine::ResManager::GetInstance()->InitBitMap("1.bmp");
	m_pFeverBack[1] = Engine::ResManager::GetInstance()->InitBitMap("2.bmp");
	m_pFeverBack[2] = Engine::ResManager::GetInstance()->InitBitMap("3.bmp");
	m_pFeverBack[3] = Engine::ResManager::GetInstance()->InitBitMap("4.bmp");

	m_pButton = new Button;
	m_pButton->Init(Engine::ResManager::GetInstance()->InitBitMap("StopButton.bmp"), 490, 0);

	for (int i = 0; i < 3; i++)
	{
		m_pFeverBar[i]->ResetSize();
	}


	iCount = 1;
	m_iAddScore = 0;
	m_iTimeCount = 0;
	m_iDelayIndex = DelayIndex;

	m_fDelay = 0;
	m_fFeverPosY = 0;
	m_fFeverDelay = 0;
	m_fFeverTextDelay = 0;
	m_fTimeDelay = 0;
	m_fFailDelay = 0;
	m_fEndDelay = 0;
	m_fTimerBarDelay = 0;
	m_fMotionDelay = 0;
	m_fTimeOverPosOne = -200;
	m_fTimeOverPosTwo = -200;

	m_bCheckFever = false;
	m_bCheckFail = false;
	m_bCheckFeverText = false;
	Fever = 0;
	m_eFeverState = FEVER_STATE_NORMAL;
	m_eFeverAniState = ANI_STATE_FRONT;

	m_iCombo = 20;

	Minute = 0;
	m_iSecond = 45;
}
void GameUI::Release()
{
	SAFE_DELETE(m_pButton);
}

bool GameUI::Updata(float fElapseTime)
{
	MotionFeverEffect(fElapseTime);
	MotionFever(fElapseTime);
	
	if (Timer(fElapseTime))
		return true;

	return false;
}
void GameUI::Draw()
{
	m_pTimerBar[0]->Draw(40, 760);
	m_pTimerBar[0]->Draw(40, 50);
	m_pTimerBar[1]->Draw(40, 763);

	m_pButton->Draw();

	m_pClockTimer->Draw(CLOCK_POS_X, CLOCK_POS_Y);
	DrawTime();

	m_pFeverBar[0]->Draw(40, 52);
	if (m_eFeverState == FEVER_STATE_FEVER)
	{
		m_pFeverBar[0]->Draw(40, 52);
		m_pFeverBar[1]->Draw(40, 52);
	}
	else if (m_eFeverState == FEVER_STATE_SUPER)
	{
		m_pFeverBar[1]->Draw(40, 52);
		m_pFeverBar[2]->Draw(40, 52);
	}
	else if (m_eFeverState == FEVER_STATE_ULTRA)
	{
		if(m_eFeverAniState == ANI_STATE_FRONT)
			m_pFeverBar[2]->Draw(40, 52);
		else if(m_eFeverAniState == ANI_STATE_BACK)
			m_pFeverBar[3]->Draw(40, 52);
	}

	
}
void GameUI::DrawWait(int Index)
{
	m_pReady[Index]->Draw(70, 200);
}
void GameUI::DrawFever()
{
	if (m_eFeverState == FEVER_STATE_NORMAL)
		return;

	m_pFeverBack[Fever]->Draw(0,0);
}
void GameUI::DrawTimeOver()
{
	m_pTimeOver[0]->Draw(100, m_fTimeOverPosOne);
	m_pTimeOver[1]->Draw(100, m_fTimeOverPosTwo);
}

void GameUI::DrawScore(int Score)
{
	if (Score == 0)
	{
		m_pNumber[0]->Draw(250, 2);
		return;
	}

	int One = -1;
	int Two = -1;
	int Thr = -1;
	int For = -1;
	int Fiv = -1;
	int Six = -1;

	if (Score / 100000 != 0)
	{
		One = Score / 100000;
		Two = (Score / 10000) % 10;
		Thr = (Score % 10000) / 1000;
		For = ((Score % 10000) % 1000) / 100;
		Fiv = (((Score % 10000) % 1000) % 100) / 10;
		Six = (((Score % 10000) % 1000) % 100) % 10;

		m_pNumber[One]->Draw(150, 2);
		m_pNumber[Two]->Draw(180, 2);
		m_pNumber[Thr]->Draw(210, 2);
		m_pComma->Draw(240, 2);
		m_pNumber[For]->Draw(270, 2);
		m_pNumber[Fiv]->Draw(300, 2);
		m_pNumber[Six]->Draw(330, 2);
	}
	else if (Score / 10000 != 0)
	{
		Two = Score / 10000;
		Thr = (Score % 10000) / 1000;
		For = ((Score % 10000) % 1000) / 100;
		Fiv = (((Score % 10000) % 1000) % 100) / 10;
		Six = (((Score % 10000) % 1000) % 100) % 10;

		m_pNumber[Two]->Draw(180, 2);
		m_pNumber[Thr]->Draw(210, 2);
		m_pComma->Draw(240, 2);
		m_pNumber[For]->Draw(270 , 2);
		m_pNumber[Fiv]->Draw(300, 2);
		m_pNumber[Six]->Draw(330, 2);

	}
	else if (Score / 1000 != 0)
	{
		Thr = Score / 1000;
		For = (Score % 1000) / 100;
		Fiv = ((Score % 1000) % 100) / 10;
		Six = ((Score % 1000) % 100) % 10;

		m_pNumber[Thr]->Draw(200, 2);
		m_pComma->Draw(230, 2);
		m_pNumber[For]->Draw(260, 2);
		m_pNumber[Fiv]->Draw(290, 2);
		m_pNumber[Six]->Draw(320, 2);
	}
	else if (Score / 100 != 0)
	{
		For = Score / 100;
		Fiv = (Score % 100) / 10;
		Six = (Score % 100) % 10;

		m_pNumber[For]->Draw(220, 2);
		m_pNumber[Fiv]->Draw(250, 2);
		m_pNumber[Six]->Draw(280, 2);
	}
	else if (Score / 10 != 0)
	{
		Fiv = (Score % 100) / 10;
		Six = (Score % 100) % 10;

		m_pNumber[Fiv]->Draw(240, 2);
		m_pNumber[Six]->Draw(270, 2);
	}
	else
		m_pNumber[Score]->Draw(250, 2);
}
bool GameUI::CheckButton()
{
	if (PtInRect(&m_pButton->GetRect(), Engine::InputManager::GetInstance()->GetMousePos()))
		return true;
	return false;
}
void GameUI::DrawTime()
{
	int One = m_iSecond / 10;
	int Two = m_iSecond % 10;
	m_pNumberSmall[One]->Draw(TIMER_NUM_POS_X, 760);
	m_pNumberSmall[Two]->Draw(TIMER_NUM_POS_X + 20, 760);

	m_pTimeColon->Draw(TIMER_NUM_POS_X + 50, 760);

	One = (int)Minute / 10;
	Two = (int)Minute % 10;
	if (Minute > 9)
	{
		m_pNumberSmall[One]->Draw(TIMER_NUM_POS_X + 80, 760);
		m_pNumberSmall[Two]->Draw(TIMER_NUM_POS_X + 100, 760);
	}
	else
	{
		m_pNumberSmall[0]->Draw(TIMER_NUM_POS_X + 80, 760);
		m_pNumberSmall[Two]->Draw(TIMER_NUM_POS_X + 100, 760);
	}
}

bool GameUI::SetSize(float fElapseTime)
{
	if (m_eFeverState == FEVER_STATE_NORMAL)
	{
		if (m_pFeverBar[FEVER_STATE_NORMAL]->SetSize(7, 0, fElapseTime))
		{
			m_eFeverState = FEVER_STATE_FEVER;
			m_bCheckFever = true;
			m_bCheckFeverText = true;
		}
	}
	else if (m_eFeverState == FEVER_STATE_FEVER)
	{
		if (m_pFeverBar[FEVER_STATE_FEVER]->SetSize(7, 0, fElapseTime))
		{
			m_eFeverState = FEVER_STATE_SUPER;
			m_bCheckFever = true;
			m_bCheckFeverText = true;
		}
	}
	else if (m_eFeverState == FEVER_STATE_SUPER)
	{
		if (m_pFeverBar[FEVER_STATE_SUPER]->SetSize(7, 0, fElapseTime))
		{
			m_eFeverState = FEVER_STATE_ULTRA;
			m_bCheckFeverText = true;
		}
	}

	m_fDelay = m_fDelay + 70 * fElapseTime;
	if (m_fDelay > 20)
	{
		m_fDelay = 0;
		return true;
	}

	return false;
}
void GameUI::SetFailFever()
{
	if (m_eFeverState == FEVER_STATE_NORMAL)
		m_bCheckFail = true;
	else
	{
		m_eFeverState = FEVER_STATE_NORMAL;
		m_bCheckFail = true;
	}
}

void GameUI::MotionFeverEffect(float fElapseTime)
{
	if (m_bCheckFail)
	{
		for (int i = 0; i < 3; i++)
			m_pFeverBar[i]->SetSize(2500, 1, fElapseTime);

		if (m_pFeverBar[0]->GetSize().cx <= 0)
		{
			m_eFeverState = FEVER_STATE_NORMAL;
			m_bCheckFail = false;
		}
	}
	if (m_bCheckFever)
	{
		m_fMotionDelay = m_fMotionDelay + 70 * fElapseTime;

		if (m_fMotionDelay > 10)
		{
			m_fMotionDelay = 0;

			Fever++;

			if (Fever == 4)
				Fever = 0;
		}
	}
}

void GameUI::MotionFever(float fElapseTime)
{
	if (m_eFeverState == FEVER_STATE_ULTRA)
	{
		m_fFeverDelay = m_fFeverDelay + 70 * fElapseTime;

		if (m_fFeverDelay > 5)
		{
			m_fFeverDelay = 0;
			if (m_eFeverAniState == ANI_STATE_FRONT)
				m_eFeverAniState = ANI_STATE_BACK;
			else if (m_eFeverAniState == ANI_STATE_BACK)
				m_eFeverAniState = ANI_STATE_FRONT;
		}
	}
	if (m_bCheckFeverText)
	{
		if (iCount % 2 == 0)
			m_fFeverPosY = m_fFeverPosY + 300 * fElapseTime;
		else
			m_fFeverPosY = m_fFeverPosY - 300 * fElapseTime;

		m_pFeverText[m_eFeverState - 1]->Draw(100, m_fFeverPosY);

		m_fFeverTextDelay = m_fFeverTextDelay + 50 * fElapseTime;

		if (m_fFeverTextDelay > 10)
		{
			m_fFeverTextDelay = 0;
			iCount++;

			if (iCount == 4)
			{
				iCount = 0;
				m_bCheckFeverText = false;
			}
		}
	}
}
bool GameUI::TimeOverAnimation(float fElapseTime)
{
	if (m_fTimeOverPosTwo < 300)
		m_fTimeOverPosTwo = m_fTimeOverPosTwo + 800 * fElapseTime;
	else if (m_fTimeOverPosOne < 310)
		m_fTimeOverPosOne = m_fTimeOverPosOne + 800 * fElapseTime;

	m_fEndDelay = m_fEndDelay + 70 * fElapseTime;

	if (m_fEndDelay > 500)
	{
		m_fEndDelay = 0;
		return true;
	}
	return false;
}

int GameUI::ScoreManager(int Score)
{
	if (Score == 0)
		return 0;

	if (Score % 6 == 0)
		return 100 * (Score / 6);

	return 0;
}
bool GameUI::Timer(float fElapseTime)
{
	m_fTimerBarDelay = m_fTimerBarDelay + m_iDelayIndex * fElapseTime;
	if (m_fTimerBarDelay > 4)
	{
		m_fTimerBarDelay = 0;
		m_pTimerBar[1]->SetBitMapSize();
	}

	Minute = Minute - 100 * fElapseTime;

	if (Minute < 4)
	{
		if (m_iSecond != 0)
			m_iSecond--;
		else if (m_iSecond == 0)
		{
			m_pTimerBar[1]->ResetSize();
			Minute = 0;
			return true;
		}

		Minute = 99;
	}
	return false;
}