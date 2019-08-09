#pragma once
#include <IncludeDefine.h>
#include <Scene.h>
#include <SceneManager.h>
#include <InputManager.h>
#include <BitMap.h>

#include "Card.h"
#include "GameMenu.h"

#define CARD_POS_X 200
#define CARD_POS_Y 375

#define MAX_COMBO 50

#define DELAY_INDEX_CARD 43

enum CARD_COLOR
{
	CARD_RED,
	CARD_BLUE,
	CARD_GREEN,
	CARD_YELLOW,
};

class GameMenu;
class GameUI;
class CardGameScene : public Engine::Scene
{
private:
	Engine::BitMap* m_pBack[2];
	Engine::BitMap* m_pInfo;
	Engine::BitMap* m_pState[2];
	Engine::BitMap* m_pEffect[6];
	Engine::BitMap* m_pComboStar[MAX_COMBO];
	GameMenu* m_pInfoMenu;
	vector <Card*> m_vcCard;

	GameUI* m_pGameUI;

	GAME_STATE m_eGameState;
	POINT m_ptMousept;
	int m_iScore;
	int m_iCount;
	int m_iCombo;
	int m_iBonusCombo;
	int m_iFieldCard;

	float m_fFeverDelay;
	float m_fDelay;
	float m_fButtonDelay;
	bool m_bCheckCombo;
	bool m_bCheckInfo;
public:
	CardGameScene();
	virtual ~CardGameScene();

	virtual void Init();
	virtual void Draw();
	virtual	bool Input(float fElapseTime);
	virtual void Updata(float fElapseTime);
	virtual void Release();
	bool UI(float fElapseTime);

	void SetCardPostion();
	void CheckCrash();
	void GenerateCard();
	void CheckCombo();
	void SettingAuto(float fElapseTime);

	void ShowInfoMenu(float fElapseTime);
	void InptInfoMenu();
};

