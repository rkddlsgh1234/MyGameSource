#pragma once
#include <IncludeDefine.h>
#include <BitMap.h>
#include <InputManager.h>
#include "CardGameScene.h"
#define BUTTON_SIZE_X 48
#define BUTTON_SIZE_Y 50

#define BUTTON_END 3
enum SCENE_STATE
{
	SCENE_TITLE,
	SCENE_GAMEMENU,
	SCENE_CARD_GAME,
	SCENE_AIRPLANE_GAME,
	SCENE_END,
};
enum BUTTON_TYPE
{
	BUTTON_TYPE_SELECT,
	BUTTON_TYPE_OK,
};
enum BUTTON_STATE
{
	BUTTON_STATE_WAIT,
	BUTTON_STATE_PUSH,
	BUTTON_STATE_DESTORY,
};

class GameMenu
{
private:
	float m_fX;
	float m_fY;

	float m_fButtonX;
	float m_fButtonY;

	float m_fButtonMotionDelay;
	int m_iCount;

	SCENE_STATE m_eGameType;
	BUTTON_TYPE m_eButtonType;
	BUTTON_STATE m_eButtonState;
	Engine::BitMap* m_pBitMap;
	Engine::BitMap* m_pButton[2];
public:
	GameMenu();
	virtual ~GameMenu();

	void Init(int x, int y, Engine::BitMap* pBitMap, SCENE_STATE GameType, BUTTON_TYPE ButtonType);
	void Draw();
	void ChangeButtonState();
	void Motion(float fElapseTime);
	void SetButtonPos(float x, float y);
	void DestoryButton();
	virtual inline RECT GetButtonSize()
	{
		RECT rc = { (LONG)m_fButtonX, (LONG)m_fButtonY, (LONG)m_fButtonX + m_pButton[0]->GetSize().cx, (LONG)m_fButtonY + m_pButton[0]->GetSize().cy };
		return rc;
	}

	virtual inline SCENE_STATE GetGameType()
	{
		return m_eGameType;
	}

	virtual inline int GetButtonCount()
	{
		return m_iCount;
	}
	virtual inline BUTTON_STATE GetButtonState()
	{
		return m_eButtonState;
	}
};
