#pragma once
#include <IncludeDefine.h>
#include <BitMap.h>

#define CARD_SIZE 140

#define CARD_SPEED 1300

enum CARD_STATE
{
	CARD_STATE_FRONT,
	CARD_STATE_BACK,
	CARD_STATE_EFFECT,
	CARD_STATE_DESTROY,
};

class Card
{
private:

	POINT m_ptPos;

	CARD_STATE m_eCardState;
	int m_iColor;
	int m_iKeyType;
	int m_iEffectCount;

	float m_fX;
	float m_fY;
	
	float m_fFailDelay;
	float m_fDelay;
	float m_fMoveDelay;
	float m_fEffectDelay;
	bool m_bCheckFever;
	bool m_bCheckScore;
	bool m_bCheckCombo;
	bool m_bCheckText;
	bool m_bCheckFail;
	bool m_bCheckFailEffect;
	Engine::BitMap* m_pBitMap;
	Engine::BitMap* m_pComboStar;
	Engine::BitMap* m_pEffect[6];

	bool m_bLife;
public:
	Card();
	~Card();

	void Init(float x, float y, Engine::BitMap*, int Color, int CardState);
	void InitComboStar(Engine::BitMap* pBitMap);
	void Draw();
	void DrawEffect();
	void Release();
	void Updata(float fElapseTime);
	void Effect(float fElapseTime);

	bool CheckCard(POINT MousePos);
	bool CheckLife();
	void MoveCard(float fElapseTime);
	void MoveFail(float fElapseTime);
	void MoveText(float fElapseTime);
	void SetFront();
	void ResetFever();
	void ResetState();


	virtual inline CARD_STATE GetState()
	{
		return m_eCardState;
	}
	virtual inline POINT ReturnPos()
	{
		return m_ptPos;
	}
	virtual inline POINT GetPos()
	{
		POINT pt = { (LONG)m_fX, (LONG)m_fY };
		return pt;
	}
	virtual inline bool GetStateText()
	{
		return m_bCheckText;
	}
	virtual inline bool GetCheckScore()
	{
		return m_bCheckScore;
	}
	virtual inline bool GetCheckFever()
	{
		return m_bCheckFever;
	}
	virtual inline CARD_STATE GetCardState()
	{
		return m_eCardState;
	}
	virtual inline bool GetCheckCombo()
	{
		return m_bCheckCombo;
	}
};

