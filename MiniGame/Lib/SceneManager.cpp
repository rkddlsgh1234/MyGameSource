#include "SceneManager.h"
#include "ResManager.h"
#include "InputManager.h"
#include "TimeManager.h"

namespace Engine
{
	SceneManager::SceneManager()
	{
	}


	SceneManager::~SceneManager()
	{
	}

	SceneManager* SceneManager::m_Instance = NULL;

	SceneManager* SceneManager::GetInstance()
	{
		if (m_Instance == NULL)
		{
			m_Instance = new SceneManager;
		}

		return m_Instance;
	}

	void SceneManager::Init(HWND hWnd, int Win_cx, int Win_cy)
	{
		srand(time(NULL));
		m_hWnd = hWnd;
		m_iWinSizeX = Win_cx;
		m_iWinSizeY = Win_cy;

		::RECT rcClient, rcWindow;
		GetClientRect(hWnd, &rcClient);
		GetWindowRect(hWnd, &rcWindow);

		HDC hdc = GetDC(m_hWnd);

		int CXFrame = (rcWindow.right - rcWindow.left) - (rcClient.right - rcClient.left);
		int CYFrame = (rcWindow.bottom - rcWindow.top) - (rcClient.bottom - rcClient.top);

		MoveWindow(hWnd, 0, 0, m_iWinSizeX + CXFrame, m_iWinSizeY + CYFrame, true);

		Engine::ResManager::GetInstance()->InitBack(hdc, m_iWinSizeX, m_iWinSizeY);
		Engine::InputManager::GetInstance()->Init(hWnd);

		ReleaseDC(m_hWnd, hdc);
		m_iScene = 0;

		m_vcScene[m_iScene]->Init();
	}
	void SceneManager::Render()
	{
		HDC hdc = GetDC(m_hWnd);
		Engine::ResManager::GetInstance()->DrawBack(hdc);
		m_vcScene[m_iScene]->Draw();
		ReleaseDC(m_hWnd, hdc);
	}
	void SceneManager::Release()
	{
		m_vcScene[m_iScene]->Release();

		for(auto i = m_vcScene.begin(); i != m_vcScene.end(); i++)
		{
			SAFE_DELETE((*i));
		}
		Engine::ResManager::GetInstance()->Release();
		Engine::InputManager::GetInstance()->Release();
		Engine::TimeManager::GetInstance()->Release();
		
		SAFE_DELETE(m_Instance);
	}

	bool SceneManager::Updata()
	{
		if (!Engine::TimeManager::GetInstance()->Update())
			return false;
		Engine::InputManager::GetInstance()->Updata();

		m_vcScene[m_iScene]->Input(TimeManager::GetInstance()->GetElipseTime());
		m_vcScene[m_iScene]->Updata(TimeManager::GetInstance()->GetElipseTime());
		
		Render();

		Engine::InputManager::GetInstance()->MouseStateUp();

		return false;
	}
	void SceneManager::GenerateScene(Engine::Scene* pScene)
	{
		m_vcScene.push_back(pScene);
	}

	void SceneManager::ChangeScene(int Game_Type)
	{
		m_vcScene[m_iScene]->Release();
		m_vcScene[Game_Type]->Init();
		
		m_iScene = Game_Type;
	}
}