#include "ComboStar.h"



ComboStar::ComboStar()
{
}


ComboStar::~ComboStar()
{
}
void ComboStar::InitEffect()
{
	m_pEffect[0] = Engine::ResManager::GetInstance()->GetBitMap("Effect_00.bmp");
	m_pEffect[1] = Engine::ResManager::GetInstance()->GetBitMap("Effect_01.bmp");
	m_pEffect[2] = Engine::ResManager::GetInstance()->GetBitMap("Effect_02.bmp");
	m_pEffect[3] = Engine::ResManager::GetInstance()->GetBitMap("Effect_03.bmp");
	m_pEffect[4] = Engine::ResManager::GetInstance()->GetBitMap("Effect_04.bmp");
	m_pEffect[5] = Engine::ResManager::GetInstance()->GetBitMap("Effect_00.bmp");

	m_fEffectDelay = 0;
	m_iEffectCount = 0;
}

void ComboStar::Effect(float fElapseTime)
{
	if (Object::GetState() != OBJECT_STATE_EFFECT)
		return;

	m_fEffectDelay = m_fEffectDelay + 70 * fElapseTime;

	if (m_fEffectDelay > 5)
	{
		m_fEffectDelay = 0;
		m_iEffectCount++;

		if (m_iEffectCount == 6)
			Object::ChangeState(OBJECT_STATE_DESTROY);
	}
}

void ComboStar::Draw()
{
	if (Object::GetState() != OBJECT_STATE_EFFECT)
	{
		Object::Draw();
		return;
	}

	m_pEffect[m_iEffectCount]->Draw((float)Object::GetPos().x, (float)Object::GetPos().y);
}