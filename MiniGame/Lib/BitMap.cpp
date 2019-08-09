#include "BitMap.h"
#include "TimeManager.h"

namespace Engine
{
	BitMap::BitMap()
	{
	}


	BitMap::~BitMap()
	{
		SelectObject(m_hMemDC, m_hOldBitMap);
		DeleteObject(m_hBitMap);
		DeleteDC(m_hMemDC);
	}

	void BitMap::InitBack(HDC hdc, int WinSizeX, int WinSizeY)
	{
		m_hMemDC = CreateCompatibleDC(hdc);
		m_hBitMap = CreateCompatibleBitmap(hdc, WinSizeX, WinSizeY);
		m_hOldBitMap = (HBITMAP)SelectObject(m_hMemDC, m_hBitMap);

		m_size.cx = WinSizeX;
		m_size.cy = WinSizeY;
	}

	void BitMap::Init(HDC hdc, string FileName)
	{
		m_hMemDC = CreateCompatibleDC(hdc);

		m_hBitMap = (HBITMAP)LoadImage(NULL, Engine::ResManager::GetInstance()->ReturnFileDirectory(FileName).c_str(), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
		m_hOldBitMap = (HBITMAP)SelectObject(m_hMemDC, m_hBitMap);
		m_strFileName = FileName;
		BITMAP bmp_info;
		GetObject(m_hBitMap, sizeof(bmp_info), &bmp_info);
		m_size.cx = (int)bmp_info.bmWidth;
		m_size.cy = (int)bmp_info.bmHeight;
	}

	void BitMap::DrawBitblt(float x, float y)
	{
		BitBlt(ResManager::GetInstance()->ReturnBackDC(), (int)x, (int)y, m_size.cx, m_size.cy, m_hMemDC, 0, 0, SRCCOPY);
	}
	void BitMap::Draw(float x, float y)
	{
		TransparentBlt(ResManager::GetInstance()->ReturnBackDC(), (int)x , (int)y, m_size.cx, m_size.cy, m_hMemDC, 0, 0, m_size.cx, m_size.cy, RGB(255, 0, 255));
	}
	void BitMap::DrawWhite(float x, float y)
	{
		TransparentBlt(ResManager::GetInstance()->ReturnBackDC(), (int)x, (int)y, m_size.cx, m_size.cy, m_hMemDC, 0, 0, m_size.cx, m_size.cy, RGB(255, 255, 255));
	}
	void BitMap::DrawBack(HDC hdc)
	{
		BitBlt(hdc, 0, 0, m_size.cx, m_size.cy, m_hMemDC, 0, 0, SRCCOPY);
	}
	HDC BitMap::ReturnDC()
	{
		return m_hMemDC;
	}
	HBITMAP BitMap::ReturnHbitmap()
	{
		return m_hBitMap;
	}

	void BitMap::SetBitMapSize()
	{
		float fSize = (float)m_size.cx - 10 * Engine::TimeManager::GetInstance()->GetElipseTime();
		m_size.cx = (LONG)fSize;
	}

	bool BitMap::SetSize(int Size, int Type, float fElapseTime)
	{	
		if (Type == 0)
		{
			if (m_size.cx + Size > 430)
			{
				m_size.cx = 430;
				return true;
			}
			else
				m_size.cx = m_size.cx + Size;
		}
		else if (Type == 1)
		{
			if (m_size.cx - Size * fElapseTime < 0)
			{
				m_size.cx = 0;
				return true;
			}
			else
			{
				float fSize = (float)m_size.cx - Size * Engine::TimeManager::GetInstance()->GetElipseTime();
				m_size.cx = (LONG)fSize;
			}
		}
		return false;
	}

	void BitMap::ResetSize()
	{
		m_size.cx = 0;
	}
}