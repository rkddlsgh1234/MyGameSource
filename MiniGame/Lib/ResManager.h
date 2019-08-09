#pragma once
#include "IncludeDefine.h"
#include "BitMap.h"

namespace Engine
{
	class BitMap;
	class ResManager
	{
	private:
		static ResManager* m_Instance;

		Engine::BitMap* m_pBack;
		vector<Engine::BitMap*> m_vcBitMap;
		string FilePath;

		ResManager();
		~ResManager();
	public:
		static ResManager* GetInstance();

		void InitBack(HDC hdc, int WinSizeX, int WinSizeY);
		Engine::BitMap* InitBitMap(string FileName);
		Engine::BitMap* GetBitMap(string FileName);

		void DrawBack(HDC hdc);
		void DrawBitMap(HDC hdc, string FileName);
		HDC ReturnBackDC();

		string ReturnFileDirectory(string FileName);

		void Release();
	};
}

