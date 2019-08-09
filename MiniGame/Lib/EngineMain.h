#pragma once
#include "IncludeDefine.h"

using namespace std;

namespace Engine
{
	class EngineMain
	{
	private:
		string m_strGameName;
		int m_iWinSizeX;
		int m_iWinSizeY;
	public:
		EngineMain(string Title, int cx, int cy);
		~EngineMain();

		int StartEngine(HINSTANCE hInstance);
	};
}

