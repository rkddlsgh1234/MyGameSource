#pragma once
#include <BitMap.h>

class BitMap;
class Button
{
private:
	Engine::BitMap* m_pButton;
	float m_fX;
	float m_fY;
public:
	Button();
	~Button();

	void Init(Engine::BitMap* pButton, float x, float y);
	void Draw();

	virtual inline RECT GetRect()
	{
		RECT rc = { (LONG)m_fX, (LONG)m_fY, (LONG)m_fX + m_pButton->GetSize().cx,  (LONG)m_fY + m_pButton->GetSize().cy };
		return rc;
	}
};