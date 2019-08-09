#pragma once
#include "cocos2d.h"

struct UserData
{
	int iMoney;
	int iCash;
	int iCharacter;
	int iJellyLevel;
	int iHpLevel;
	int iTimeLevel;
};

class GameManager
{
private:
	static GameManager* m_Instance;

	UserData* m_stUserData;
	int m_iMaxScore;
	int m_iGetCoin;
	int m_iGetJelly;

	GameManager();
	~GameManager();
public:
	static GameManager* GetInstance();

	void Init(UserData* stUserData);
	void SetMoney(int iMoney);
	void SetJellyLevel(int iJelly);
	void SetHpLevel(int iLevel);
	void SetScore(int iMoney, bool bPlus);
	void SetJellyScore(int iLevel, bool bPlus);
	void ResetScore();

	virtual inline UserData* GetUserData()
	{
		return m_stUserData;
	}
	virtual inline int GetMaxScore()
	{
		return m_iMaxScore;
	}
	virtual inline int GetCoin()
	{
		return m_iGetCoin;
	}
	virtual inline int GetJelly()
	{
		return m_iGetJelly;
	}
};