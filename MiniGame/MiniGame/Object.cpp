#include "Object.h"
#include "AirplaneScene.h"


Object::Object()
{
}


Object::~Object()
{
}

void Object::Init(Engine::BitMap* pBitMap, int Random, POINT Pos)
{
	m_pBitMap = pBitMap;
	m_bLife = true;
	m_eWay = (STATE_WAY)Random;
	m_eState = OBJECT_STATE_LIFE;
	m_fX = (float)Pos.x;
	m_fY = (float)Pos.y;
}
void Object::Draw()
{
	if (m_eState == OBJECT_STATE_DESTROY)
		return;

	m_pBitMap->Draw(m_fX, m_fY);
}


void Object::Move(float fElapseTime, int SpeedOne, int SpeedTwo)
{
	if (m_eState != OBJECT_STATE_LIFE)
		return;

	if (m_eWay == LEFT_UP)
	{
		m_fX = m_fX - SpeedOne * fElapseTime;
		m_fY = m_fY - SpeedTwo * fElapseTime;
	}
	else if (m_eWay == LEFT_DOWN)
	{
		m_fX = m_fX - SpeedOne * fElapseTime;
		m_fY = m_fY + SpeedTwo * fElapseTime;
	}
	else if (m_eWay == RIGHT_UP)
	{
		m_fX = m_fX + SpeedOne * fElapseTime;
		m_fY = m_fY - SpeedTwo * fElapseTime;
	}
	else if (m_eWay == RIGHT_DOWN)
	{
		m_fX = m_fX + SpeedOne * fElapseTime;
		m_fY = m_fY + SpeedTwo * fElapseTime;
	}
	else if (m_eWay == DOWN_LEFT)
	{
		m_fX = m_fX - SpeedTwo * fElapseTime;
		m_fY = m_fY + SpeedOne * fElapseTime;
	}
	else if (m_eWay == DOWN_RIGHT)
	{
		m_fX = m_fX + SpeedTwo * fElapseTime;
		m_fY = m_fY + SpeedOne * fElapseTime;
	}
	else if (m_eWay == DOWN)
	{
		m_fY = m_fY + SpeedOne * fElapseTime;
	}
}

void Object::DestroyObject()
{
	if (m_eState != OBJECT_STATE_LIFE)
		return;

	if ( m_eWay == LEFT_UP)
	{
		if(m_fX < FIELD_LEFT || m_fY + m_pBitMap->GetSize().cy < FIELD_TOP)
			m_eState = OBJECT_STATE_DESTROY;
	}
	else if (m_eWay == RIGHT_UP)
	{
		if (m_fX  > FIELD_RIGHT || m_fY + m_pBitMap->GetSize().cy < FIELD_TOP)
			m_eState = OBJECT_STATE_DESTROY;
	}
	else if (m_eWay == DOWN_LEFT || m_eWay == LEFT_DOWN)
	{
		if (m_fX + m_pBitMap->GetSize().cx < FIELD_LEFT || m_fY + m_pBitMap->GetSize().cy < FIELD_TOP)
			m_eState = OBJECT_STATE_DESTROY;
	}
	else if (m_eWay == DOWN_RIGHT || m_eWay == RIGHT_DOWN)
	{
		if (m_fX > FIELD_RIGHT || m_fY > FIELD_BOTTOM)
			m_eState = OBJECT_STATE_DESTROY;
	}
}
void Object::Destory()
{
	m_fY -= 20;
	m_eState = OBJECT_STATE_DESTROY;
}
void Object::Crash()
{
	m_eState = OBJECT_STATE_EFFECT;
}
void Object::Hide()
{
	if (m_eState == OBJECT_STATE_DESTROY)
		return;
	m_eState = OBJECT_STATE_DESTROY;
	m_bLife = false;
}
void Object::ChangeBitMap(Engine::BitMap* pBitMap)
{
	if (m_eState != OBJECT_STATE_LIFE)
		return;

	m_pBitMap = pBitMap;
}

void Object::ChangeState(OBJECT_STATE eState)
{
	m_eState = eState;
}
void Object::SetPos(float x, float y)
{
	if(x != NULL)
		m_fX = x;
	if (y != NULL)
		m_fY = y;
}

void Object::InitEffect()
{

}
void Object::Effect(float fElapseTime)
{

}
void Object::MoveTo()
{

}

