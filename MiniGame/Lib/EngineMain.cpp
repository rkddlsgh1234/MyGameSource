#include "EngineMain.h"
#include "SceneManager.h"
#include "InputManager.h"
namespace Engine
{
	EngineMain::EngineMain(string Title, int cx, int cy)
	{
		m_strGameName = Title;
		m_iWinSizeX = cx;
		m_iWinSizeY = cy;
	}


	EngineMain::~EngineMain()
	{
	}
	
	LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	int EngineMain::StartEngine(HINSTANCE hInstance)
	{
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

		MSG Message;
		WNDCLASS WndClass;

		WndClass.cbClsExtra = 0;
		WndClass.cbWndExtra = 0;
		WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		WndClass.hInstance = hInstance;
		WndClass.lpfnWndProc = WndProc;
		WndClass.lpszClassName = m_strGameName.c_str();
		WndClass.lpszMenuName = NULL;
		WndClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
		RegisterClass(&WndClass);

		HWND hWnd = CreateWindow(m_strGameName.c_str(), m_strGameName.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
			CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, hInstance, NULL);
		ShowWindow(hWnd, SW_SHOWDEFAULT);

		SceneManager::GetInstance()->Init(hWnd, m_iWinSizeX, m_iWinSizeY);
		
		while (true)
		{

			/// 메시지큐에 메시지가 있으면 메시지 처리
			if (PeekMessage(&Message, NULL, 0U, 0U, PM_REMOVE))
			{
				if (Message.message == WM_QUIT)
					break;

				TranslateMessage(&Message);
				DispatchMessage(&Message);
			}
			else
			{
				if (SceneManager::GetInstance()->Updata())
				{
					SendMessage(hWnd, WM_DESTROY, 0, 0);
				}
			}
		}
		Engine::SceneManager::GetInstance()->Release();
		Engine::EngineMain::~EngineMain();
		_CrtDumpMemoryLeaks();

		return (int)Message.wParam;
	}

	LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
	{
		switch (iMessage)
		{
		case WM_LBUTTONUP:
			Engine::InputManager::GetInstance()->InputMouse();
			return 0;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		}

		return(DefWindowProc(hWnd, iMessage, wParam, lParam));
	}
}
