#pragma once
#include "IncludeDefine.h"
#include "ResManager.h"

using namespace std;

namespace Engine
{
	class BitMap
	{
	public:
		HDC		m_hMemDC;
		HBITMAP	m_hBitMap;
		HBITMAP	m_hOldBitMap;
		SIZE m_size;
		string m_strFileName;
	public:
		void InitBack(HDC hdc, int WinSizeX, int WinSizeY);
		void Init(HDC hdc, string FileName);

		void Draw(float x, float y);
		void DrawWhite(float x, float y);
		void DrawBitblt(float x, float y);
		void DrawBack(HDC hdc);
		
		HDC ReturnDC();
		HBITMAP ReturnHbitmap();

		void SetBitMapSize();
		bool SetSize(int Size, int Type, float fElapseTime);
		void ResetSize();

		virtual inline string GetFileName()
		{
			return m_strFileName;
		}
		virtual inline SIZE GetSize()
		{
			return m_size;
		}
		virtual inline HBITMAP GetBitMap()
		{
			return m_hBitMap;
		}

		BitMap();
		~BitMap();
	};
}
