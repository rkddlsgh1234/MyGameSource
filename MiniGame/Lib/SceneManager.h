#pragma once
#include "IncludeDefine.h"
#include "Scene.h"

namespace Engine
{
	class Scene;
	class SceneManager
	{
	private:
		HWND m_hWnd;
		static SceneManager* m_Instance;	

		vector<Engine::Scene*> m_vcScene;
		int m_iWinSizeX;
		int m_iWinSizeY;

		int m_iScene;
		int m_iBackScene;

		SceneManager();
		~SceneManager();
	public:
		static SceneManager* GetInstance();

		void Init(HWND hWnd, int Win_cx, int Win_cy);
		void Render();
		bool Updata();
		void Release();
		void GenerateScene(Engine::Scene* pScene);
		void ChangeScene(int Game_Type);
		
		virtual inline HWND GethWnd()
		{
			return m_hWnd;
		}
	};
}

