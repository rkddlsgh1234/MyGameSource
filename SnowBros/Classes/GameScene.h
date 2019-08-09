#pragma once
#include "cocos2d.h"
#include "Playerble.h"
#include "Enemy.h"
#include "Boss.h"
#include <vector>

#define SAFE_DELETE(pObject) if((pObject) != NULL) {delete (pObject); (pObject) = NULL;}

#define __GAMESCENE_SCENE_H__

using namespace std;

enum GAME_STAGE
{
	GAME_STAGE_ONE,
	GAME_STAGE_TWO,
	GAME_STAGE_THR,
};

class GameScene : public cocos2d::Scene
{
private:
	Playerble* m_pPlayer;
	GAME_STAGE m_eGameStage;
	int m_iScore;
	int m_iLife;
	int m_iCombo;
	bool m_bKeyPush;
	bool m_bStart;
	bool m_bGameOver;
	bool m_bEnd;
	bool m_bEndRound;
	cocos2d::Sprite* m_pUi_Player;
	cocos2d::Sprite* m_pUi_LifeIcon;
	cocos2d::Label* m_pUi_ScoreLabel;
	cocos2d::Label* m_pUi_LifeLabe;
	cocos2d::EventKeyboard::KeyCode m_iKeyCode;
private:
	cocos2d::Sprite* m_pStageBg_00;
	cocos2d::Vector <cocos2d::Sprite*> m_vcStageTile_00;
	cocos2d::Vector <Enemy*> m_vcEnemy_00;
	cocos2d::Sprite* m_pStageBg_01;
	cocos2d::Vector <cocos2d::Sprite*> m_vcStageTile_01;
	cocos2d::Vector<Enemy*> m_vcEnemy_01;
	cocos2d::Sprite* m_pStageBg_02;
	cocos2d::Vector <cocos2d::Sprite*> m_vcStageTile_02;
	cocos2d::Vector<Enemy*> m_vcEnemy_02;
	Boss* m_pBoss;

	cocos2d::Sprite* m_pStageBg;
	cocos2d::Vector <cocos2d::Sprite*> m_vcStageTile;
	cocos2d::Vector <cocos2d::Sprite*> m_vcItem;


	cocos2d::Vector <Enemy*> m_vcEnemy;
private:
	cocos2d::SpriteFrameCache* m_pStageCache_00;
	cocos2d::SpriteFrameCache* m_pStageCache_01;
	cocos2d::SpriteFrameCache* m_pStageCache_02;
	cocos2d::SpriteFrameCache* m_pBossCache_00;
	cocos2d::SpriteFrameCache* m_pMobRedCache;
	cocos2d::SpriteFrameCache* m_pMobBeastCache;
	cocos2d::SpriteFrameCache* m_pEffectCache;
	cocos2d::SpriteFrameCache* m_pItemCache;
	cocos2d::SpriteFrameCache* m_pIceCache;
	cocos2d::SpriteFrameCache* m_pNumCache;
	cocos2d::SpriteFrameCache* m_pUICache;
public:
	GameScene();
	~GameScene();

	static cocos2d::Scene* createScene();
	virtual bool init();
	void Updata(float DelayTime);
	
	void StartNextRound();
	void StartGame();
	void StartSound();
	void SpawnPlayer();
	void ReSpawnPlayer(float DelayTime);
	void SetGodMode(float DelayTime);

	void InitCache();
	void InitSprite();
	void PressKeyboard(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void ReleaseKeyboard(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

	void GenerateEnemyBoss(float DelayTime);

	void MoveEnemy();

	void ShowGameOver();
	void CheckClearRound();
	void ChangeRound(float DelayTime);
	void EndRound();
	void BackTitleScene(float DelayTime);

	void SetGravity();
	void SetGravityEnemy();
	void SetGravityItem();
	void SetGravityBoss();

	bool CheckCrashTile();
	void CheckCrashBulletToEnemy();
	void CheckCrashPlayerToEnemy();
	void CheckCrashIceEnemyToEnemy();
	void CheckCrashItem();
	void CheckCrashPlayerToItem();
	void CheckCrashBossToPlayer();
	void CheckCrashEnemyToBoss();
	void CheckDeadBoss();

	CREATE_FUNC(GameScene);
};