#pragma once
#include <BitMap.h>

#define BULLET_SIZE 20
#define STAR_SIZE 100

enum STATE_WAY
{
	DOWN_LEFT,
	DOWN_RIGHT,
	LEFT_UP,
	LEFT_DOWN,
	RIGHT_UP,
	RIGHT_DOWN,
	END,
	UP,
	DOWN,
};
enum OBJECT_STATE
{
	OBJECT_STATE_LIFE,
	OBJECT_STATE_DESTROY,
	OBJECT_STATE_EFFECT,
};
enum OBJECT_TYPE
{
	OBJECT_TYPE_BULLET,
	OBJECT_TYPE_STAR,
};
class Object
{
private:
	Engine::BitMap* m_pBitMap;
	float m_fX;
	float m_fY;
	bool m_bLife;
	STATE_WAY m_eWay;
	OBJECT_STATE m_eState;
public:
	Object();
	~Object();

	void Init(Engine::BitMap* pBitMap, int Random, POINT Pos);
	virtual void Draw();
	void Move(float fElapseTime, int SpeedOne, int SpeedTwo);

	void DestroyObject();
	void Destory();
	void ChangeBitMap(Engine::BitMap* pBitMap);
	void Hide();
	void Crash();
	void SetPos(float x, float y);

	virtual void InitEffect();
	virtual void Effect(float fElapseTime);
	virtual void MoveTo();
	

	void ChangeState(OBJECT_STATE);

	virtual inline OBJECT_STATE GetState()
	{
		return m_eState;
	}
	virtual inline bool GetLife()
	{
		return m_bLife;
	}
	virtual inline RECT GetRect()
	{
		RECT rc = { (LONG)m_fX, (LONG)m_fY, (LONG)m_fX + m_pBitMap->GetSize().cx, (LONG)m_fY + m_pBitMap->GetSize().cy};
		return rc;
	}
	virtual inline POINT GetPos()
	{
		POINT pt = { (LONG)m_fX, (LONG)m_fY };
		return pt;
	}
};

