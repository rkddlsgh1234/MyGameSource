#pragma once
#include "Object.h"

#define EFFECT_INDEX 6

class ComboStar : public Object
{	
private:
	Engine::BitMap* m_pEffect[6];
	int m_iEffectCount;
	float m_fEffectDelay;
public:
	ComboStar();
	~ComboStar();

	virtual void Draw();
	virtual void InitEffect();
	virtual void Effect(float fElapseTime);
};

