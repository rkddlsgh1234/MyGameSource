#include "InputManager.h"

namespace Engine
{
	InputManager::InputManager()
	{
		m_bMouseDown = false;
		m_bCheckPress = false;
	}


	InputManager::~InputManager()
	{
	}
	InputManager* InputManager::m_Instance = NULL;

	InputManager* InputManager::GetInstance()
	{
		if (m_Instance == NULL)
		{
			m_Instance = new InputManager;
		}

		return m_Instance;
	}

	void InputManager::Init(HWND hWnd)
	{
		m_hWnd = hWnd;
		MouseStateUp();
	}


	void InputManager::Updata()
	{
	
	}
	void InputManager::Release()
	{
		SAFE_DELETE(m_Instance);
	}

	void InputManager::InputMouse()
	{
		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(m_hWnd, &pt);
		m_ptMousePos.x = pt.x;
		m_ptMousePos.y = pt.y;
		m_bMouseDown = true;
	}
	POINT InputManager::ReturnMouseCusur()
	{
		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(m_hWnd, &pt);

		return pt;
	}
	void InputManager::MouseStateUp()
	{
		m_bMouseDown = false;
		m_ptMousePos.x = NULL;
		m_ptMousePos.y = NULL;
	}
}