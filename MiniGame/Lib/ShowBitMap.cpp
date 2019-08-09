#include "ShowBitMap.h"

namespace Engine
{
	ShowBitMap::ShowBitMap()
	{
	}


	ShowBitMap::~ShowBitMap()
	{
	}

	void ShowBitMap::Init(Engine::BitMap* pBitMap, float x, float y)
	{
		m_pBitMap = pBitMap;
		m_iX = x;
		m_iY = y;
		m_rcSize = { 0, 0, pBitMap->GetSize().cx, pBitMap->GetSize().cy};
	}
	void ShowBitMap::Draw()
	{
		m_pBitMap->Draw(m_iX, m_iY);
	}
	
	void ShowBitMap::SetPos(int x, int y)
	{
		if (y < 110 || y > 700)
		{
			if (x < 0 || x > 470)
				return;
			else
				m_iX = x;	
		}
		else if (x < 0 || x > 470)
		{
			if (y < 110 || y > 700)
				return;
			else
				m_iY = y;
		}
		else
		{
			m_iX = x;
			m_iY = y;
		}
	}
}