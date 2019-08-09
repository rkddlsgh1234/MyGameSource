#include "Card.h"
#include "CardGameScene.h"


Card::Card()
{
	m_iKeyType = NULL;
}


Card::~Card()
{
}

void Card::Init(float x, float y, Engine::BitMap* pBitMap, int Color, int CardState)
{
	m_fX = x;
	m_fY = y;
	m_pBitMap = pBitMap;
	m_iColor = Color;

	m_eCardState = (CARD_STATE)CardState;

	m_bLife = false;
	m_bCheckScore = false;
	m_bCheckText = false;
	m_bCheckFever = false;
	m_bCheckCombo = false;
	m_bCheckFail = false;
	m_bCheckFailEffect = false;

	m_iEffectCount = 0;

	m_fMoveDelay = 0;
	m_fDelay = 0;
	m_fEffectDelay = 0;
	m_fFailDelay = 0;

	if (m_iColor == CARD_RED)
		m_ptPos = { (LONG)375 + 20, (LONG)m_fY - 20};
	else if (m_iColor == CARD_BLUE)
		m_ptPos = { (LONG)20 + 20, (LONG)m_fY - 20};
	else if (m_iColor == CARD_GREEN)
		m_ptPos = { (LONG)m_fX + 20, (LONG)180 - 20};
	else if (m_iColor == CARD_YELLOW)
		m_ptPos = { (LONG)m_fX + 20, (LONG)500 - 20};
}
void Card::InitComboStar(Engine::BitMap* pBitMap)
{
	m_pComboStar = pBitMap;
	m_pEffect[0] = Engine::ResManager::GetInstance()->GetBitMap("Effect_00.bmp");
	m_pEffect[1] = Engine::ResManager::GetInstance()->GetBitMap("Effect_01.bmp");
	m_pEffect[2] = Engine::ResManager::GetInstance()->GetBitMap("Effect_02.bmp");
	m_pEffect[3] = Engine::ResManager::GetInstance()->GetBitMap("Effect_03.bmp");
	m_pEffect[4] = Engine::ResManager::GetInstance()->GetBitMap("Effect_04.bmp");
	m_pEffect[5] = Engine::ResManager::GetInstance()->GetBitMap("Effect_05.bmp");
	m_bCheckCombo = true;
}

void Card::Draw()
{
	if (m_eCardState == CARD_STATE_DESTROY)
		return;

	m_pBitMap->Draw(m_fX, m_fY);

	if(m_bCheckCombo)
		m_pComboStar->Draw(m_fX + 20, m_fY + 20);
}
void Card::DrawEffect()
{
	if (m_eCardState != CARD_STATE_EFFECT)
		return;

	m_pEffect[m_iEffectCount]->Draw(m_fX, m_fY);
}

void Card::Updata(float fElapseTime)
{
	MoveCard(fElapseTime);
	MoveText(fElapseTime);
	MoveFail(fElapseTime);
	Effect(fElapseTime);
}



void Card::Effect(float fElapseTime)
{
	if (m_eCardState != CARD_STATE_EFFECT)
		return;

	m_fEffectDelay = m_fEffectDelay + 70 * fElapseTime;

	if (m_fEffectDelay > 5)
	{
		m_fEffectDelay = 0;
		m_iEffectCount++;

		if (m_iEffectCount == 6)
		{
			m_iEffectCount = 0;
			m_eCardState = CARD_STATE_DESTROY;
		}
	}
}

bool Card::CheckCard(POINT MousePos)
{
	RECT rc;
	if (m_iColor == CARD_RED)
		rc = { (LONG)375, (LONG)m_fY, (LONG)375 + CARD_SIZE, (LONG)m_fY + CARD_SIZE };
	else if (m_iColor == CARD_BLUE)
		rc = { (LONG)20, (LONG)m_fY, (LONG)20 + CARD_SIZE, (LONG)m_fY + CARD_SIZE };
	else if (m_iColor == CARD_GREEN)
		rc = { (LONG)m_fX, (LONG)180, (LONG)m_fX + CARD_SIZE, (LONG)180 + CARD_SIZE };
	else if (m_iColor == CARD_YELLOW)
		rc = { (LONG)m_fX, (LONG)500, (LONG)m_fX + CARD_SIZE, (LONG)500 + CARD_SIZE };

	if (PtInRect(&rc, MousePos))
	{
		m_bCheckScore = true;
		m_bCheckText = true;
		m_bCheckFever = true;
	}

	return m_bCheckScore;
}

void Card::MoveCard(float fElapseTime)
{
	if (m_bCheckScore != true)
		return;

	m_fMoveDelay = m_fMoveDelay + 70 * fElapseTime;
	float Pos = m_ptPos.y - 20 * fElapseTime;
	m_ptPos.y = (LONG)Pos;

	if (m_iColor == CARD_RED)
		m_fX = m_fX + (CARD_SPEED - 150) * fElapseTime;
	else if (m_iColor == CARD_BLUE)
		m_fX = m_fX - (CARD_SPEED - 100) * fElapseTime;
	else if (m_iColor == CARD_GREEN)
		m_fY = m_fY - CARD_SPEED * fElapseTime;
	else if (m_iColor == CARD_YELLOW)
		m_fY = m_fY + (CARD_SPEED - 150) * fElapseTime;

	if (m_fMoveDelay > 10)
	{
		m_fMoveDelay = 0;
		m_bCheckScore = false;

		m_bLife = true;
		if (m_bCheckCombo)
			m_eCardState = CARD_STATE_EFFECT;
		else
			m_eCardState = CARD_STATE_DESTROY;
	}
}

void Card::MoveText(float fElapseTime)
{
	if (m_bCheckText != true)
		return;

	m_fDelay = m_fDelay + 70 * fElapseTime;

	if (m_fDelay > 20)
	{
		m_fDelay = 0;
		m_bCheckText = false;
		m_bCheckCombo = false;
	}
}
void Card::MoveFail(float fElapseTime)
{
	if (m_bCheckFail == false)
		return;

	m_fFailDelay = m_fFailDelay + 70 * fElapseTime;

	if (m_bCheckFailEffect)
	{
		m_bCheckFailEffect = false;
		m_fX += 2;
	}
	else
	{
		m_bCheckFailEffect = true;
		m_fX -= 2;
	}

	if (m_fFailDelay > 30)
	{
		m_fFailDelay = 0;
		m_fX = CARD_POS_X;
		m_bCheckFail = false;
		m_bCheckFailEffect = false;
	}
}

void Card::Release()
{
	SAFE_DELETE(m_pBitMap);
}


void Card::ResetFever()
{
	m_bCheckFever = false;
}
void Card::ResetState()
{
	m_bCheckCombo = false;
	m_bCheckFail = true;
}

void Card::SetFront()
{
	if (m_eCardState == CARD_STATE_DESTROY)
		return;
	if (m_bCheckScore)
		return;

	m_fX -= 5;
	m_fY -= 5;
	m_eCardState = CARD_STATE_FRONT;
}

bool Card::CheckLife()
{
	if (m_bLife != true && m_bCheckScore != true)
		return true;

	return false;
}