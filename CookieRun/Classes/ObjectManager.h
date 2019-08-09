#pragma once
#include "cocos2d.h"
using namespace std;

enum GAME_STAGE
{
	GAME_STAGE_ONE,
	GAME_STAGE_TWO,
	GAME_STAGE_THR,
};

enum ENEMY_OBJECT
{
	STAGE_00_ENEMY_OBJECT_UP,
	STAGE_00_ENEMY_OBJECT_DOWN_SHORT,
	STAGE_00_ENEMY_OBJECT_DOWN_LONG,
	STAGE_01_ENEMY_OBJECT_UP,
	STAGE_01_ENEMY_OBJECT_DOWN_SHORT,
	STAGE_01_ENEMY_OBJECT_DOWN_LONG,
	STAGE_02_ENEMY_OBJECT_UP,
	STAGE_02_ENEMY_OBJECT_DOWN_SHORT,
	STAGE_02_ENEMY_OBJECT_DOWN_LONG,
};


#define BACK_SPEED 8
#define BOOST_SPEED 20
#define STAGE_00_ROAD 150
#define STAGE_01_ROAD 155

#define JELLY_POS_JUMP 8
#define JELLY_POS_DUBBLE_JUMP 14

class ObjectManager
{
private:
	cocos2d::Scene* m_pScene;
	cocos2d::Sprite* m_pPlayer;
	GAME_STAGE m_eGameStage;

	float m_fBackSize;
	int m_iStageRoofCount;
	int m_iStageRoofMax;
	int m_iSpeed;

	int m_iBounsTime;

	bool m_bStartState;
	bool m_bFever;
private:
	cocos2d::Sprite* m_pDelay;
private:
	cocos2d::Sprite* m_pLoadingBackGround;
	cocos2d::Sprite* m_pLoadingBackEffect;
	cocos2d::Sprite* m_pLoadingCharacter;
private:
	cocos2d::Vector <cocos2d::Sprite*> m_vcGameBackGround;
	cocos2d::Vector <cocos2d::Sprite*> m_vcStageRoad;
	cocos2d::Vector <cocos2d::Sprite* > m_vcEnemy;
	cocos2d::Vector <cocos2d::Sprite*> m_vcItem;
	cocos2d::Vector <cocos2d::Sprite*> m_vcPoint;
	cocos2d::Vector <cocos2d::Sprite*> m_vcBonusBackGround;
	cocos2d::Vector <cocos2d::Sprite*> m_vcBonusItem;

	cocos2d::Vector <cocos2d::Sprite*> m_vcBackGroundStage00;
	cocos2d::Vector <cocos2d::Sprite*> m_vcBackGroundStage01;
	cocos2d::Vector <cocos2d::Sprite*> m_vcBackGroundStage02;
private:
	cocos2d::SpriteFrameCache* m_pCharacterCache;
	cocos2d::SpriteFrameCache* m_pLoadingCache;
	cocos2d::SpriteFrameCache* m_pStageCache_00;
	cocos2d::SpriteFrameCache* m_pStageCache_01;
	cocos2d::SpriteFrameCache* m_pStageCache_02;
	cocos2d::SpriteFrameCache* m_pBonusCache;

private:

public:
	ObjectManager();
	~ObjectManager();

	void Init(cocos2d::Scene* pScene);
	void InitPlayer(cocos2d::Sprite* pPlayer);
	void InitCache();
	void InitLoading();
	void InitPlayGame();
	void InitStart();
	void SetObject(cocos2d::Sprite* pSprite);
	void SetItem(cocos2d::Sprite* pSprite);
	void SetPoint(cocos2d::Sprite* pSprite);

	void GenerateBonusTime();
	void GenerateBonusJelly();

	bool Updata();
	void MoveObject();

	void CheckNextChage();
	bool CheckBackGroundRoof(cocos2d::Sprite* pBackGround);
	bool CheckCrashRoad(cocos2d::Sprite* pPlayer);
	bool CheckCrashCookie(cocos2d::Sprite* pPlayer);
	bool CheckGetItem(cocos2d::Sprite* pPlayer);
	bool CheckGetJelly(cocos2d::Sprite* pPlayer);
	bool CheckCrashDestroy(cocos2d::Sprite* pPlayer);
	bool CheckCrashMove(cocos2d::Sprite* pPlayer);

	void ChangeSceneFade();
	void ChangeBackGround();
	void ChangeMaxSpeed();
	void ChangeMinSpeed();
	void ChangeFeverMode();
	void UnChangeFeverMode();

	virtual inline bool GetGameState()
	{
		return m_bStartState;
	}
	virtual inline int GetSpeed()
	{
		return m_iSpeed;
	}
	virtual inline cocos2d::Sprite* GetPlayer()
	{
		return m_pPlayer;
	}
};