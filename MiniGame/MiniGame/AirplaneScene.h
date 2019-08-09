#pragma once
#include <IncludeDefine.h>
#include <Scene.h>
#include <SceneManager.h>
#include <BitMap.h>
#include <InputManager.h>

#include "GameMenu.h"

#define BULLET_GENERATE_DELAY 40
#define STAR_GENERATE_NORMAL_DELAY 90
#define STAR_GENERATE_FEVER_DELAY 75
#define STAR_GENERATE_SPUER_DELAY 60
#define STAR_GENERATE_ULTRA_DELAY 45
#define SCORE 100

#define EMPTY 0
#define AIRPLANE_SIZE_X 73
#define AIRPLANE_SIZE_Y 73
#define FEVER_STAR_NUM 5

#define FIELD_TOP 80
#define FIELD_LEFT 0
#define FIELD_BOTTOM 750
#define FIELD_RIGHT 540

#define DELAY_INDEX_AIRPLANE 43

class Object;
class GameMenu;
class ComboStar;
class Bullet;
class Airplane;
class Cloud;
class GameUI;
class BitMap;
class AirplaneScene : public Engine::Scene
{
private:
	Engine::BitMap* m_pBack[2];
	Airplane* m_pAirplane;
	GameUI * m_pGameUI;

	GAME_STATE m_eGameState;
	
	vector <Object*> m_vcCloud;
	vector <Object*> m_vcBullet;
	vector <Object*> m_vcObject;
	vector <Object*> m_vcStar;
	GameMenu* m_pInfoMenu;

	int m_iCount;
	int m_iScore;
	int m_iBonusCombo;
	int m_iGenerateStarDelayIndex;
	int m_iSpeedX;
	int m_iSpeedY;

	float m_fDelay;
	float m_fPointDelay;
	float m_fFeverStateDelay;
	float m_fFeverGaugeDelay;
	float m_fGenearteBulletDelay;
	float m_fGenerateStarDelay;
	float m_fFeverDelay;

	bool m_bCheckInfo;
public:
	AirplaneScene();
	virtual ~AirplaneScene();

	virtual void Init();
	virtual void Draw();
	virtual	bool Input(float fElapseTime);
	virtual void Updata(float fElapseTime);
	virtual void Release();
	bool UI(float fElapseTime);

	void GenerateBullet(float fElapseTime);
	void GenerateStar(float fElapseTime);
	void CheckCrash();
	void ResetObject();
	void SettingAuto(float fElapseTime);
	void Action(float fElapseTime);
	void SettingScore(float fElapseTime);
	void ChangeSpeed();
	void Reset();
	POINT SetRandomPos(int RandomWay, int Type);

	void ShowInfoMenu(float fElapseTime);
	void InptInfoMenu();
};

