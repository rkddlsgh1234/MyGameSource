#include <Windows.h>
#include "resource.h"
#include "MapToolManager.h"
#include "ResManager.h"
#include "GameManager.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR IpszClass = TEXT("Battle City");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR IpszCmdParam, int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtDumpMemoryLeaks();
	//_CrtSetBreakAlloc(757);

	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = IpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);
	
	hWnd = CreateWindow(IpszClass, IpszClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);

	/*
	while (GetMessage(&Message, NULL, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
	
	*/
	
	GameManager::GetInstance()->Init(hWnd, g_hInst);

	while (true)
	{
		if (PeekMessage(&Message, NULL, 0U, 0U, PM_REMOVE))
		{
			if (Message.message == WM_QUIT)
				break;
			TranslateMessage(&Message);
			DispatchMessage(&Message);

		}
		else
			GameManager::GetInstance()->Updata();
		
	}
	return (int)Message.wParam;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	switch (iMessage)
	{
	case WM_LBUTTONDOWN:
		MapToolManager::GetInstance()->DrawTile(LOWORD(lParam), HIWORD(lParam));
		MapToolManager::GetInstance()->SetDrawTool(LOWORD(lParam), HIWORD(lParam));
		InvalidateRect(hWnd, NULL, FALSE);
		return 0;
	case WM_RBUTTONDOWN:
		MapToolManager::GetInstance()->ReMoveTile(LOWORD(lParam), HIWORD(lParam));
		InvalidateRect(hWnd, NULL, FALSE);
		return 0;
	case WM_KEYDOWN:
		switch (wParam) 
		{
		case VK_ESCAPE:
			GameManager::GetInstance()->ChangeState();
			break;
		}
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case 50:		
			MapToolManager::GetInstance()->SaveFile();
			return 0;
		case 51:
			MapToolManager::GetInstance()->LoadFile();
			return 0;
		case VK_ESCAPE:
			GameManager::GetInstance()->ChangeState();
			break;
		}
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		MapToolManager::GetInstance()->Draw();
		EndPaint(hWnd, &ps);

		return 0;
	case WM_DESTROY:
		MapToolManager::GetInstance()->Release();
		GameManager::GetInstance()->Release();
		PostQuitMessage(0);
		return 0;
	}
	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}