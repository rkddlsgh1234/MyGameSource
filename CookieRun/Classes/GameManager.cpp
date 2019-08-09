#include "GameManager.h"

USING_NS_CC;

GameManager::GameManager()
{
}


GameManager::~GameManager()
{
	delete m_stUserData;
	delete m_Instance;
}
GameManager* GameManager::m_Instance = NULL;

GameManager* GameManager::GetInstance()
{
	if (m_Instance == NULL)
	{
		m_Instance = new GameManager;
	}

	return m_Instance;
}

void GameManager::Init(UserData* stUserData)
{
	m_stUserData = stUserData;
}
void GameManager::ResetScore()
{
	m_iGetCoin = 0;
	m_iGetJelly = 0;
}
void GameManager::SetMoney(int iMoney)
{
	m_stUserData->iMoney = iMoney;
}
void GameManager::SetHpLevel(int iLevel)
{
	m_stUserData->iHpLevel = iLevel;
}
void GameManager::SetJellyLevel(int iLevel)
{
	m_stUserData->iJellyLevel = iLevel;
}
void GameManager::SetScore(int iMoney, bool bPlus)
{
	if(bPlus)
		m_iGetCoin += iMoney;
	else
		m_iGetCoin -= iMoney;
}
void GameManager::SetJellyScore(int iJelly, bool bPlus)
{
	if (bPlus)
		m_iGetJelly += iJelly;
	else
		m_iGetJelly -= iJelly;
}