#pragma once
#include "IncludeDefine.h"

namespace Engine
{
	class InputManager
	{
	private:
		static InputManager* m_Instance;
		bool m_bCheckPress;

		HWND m_hWnd;
		POINT m_ptMousePos;
		bool m_bMouseDown;

		InputManager();
		~InputManager();
	public:
		static InputManager* GetInstance();

		void Init(HWND hWnd);
		void Updata();
		void Release();

		void InputMouse();
		void MouseStateUp();
		POINT ReturnMouseCusur();
		
		virtual inline bool GetMousePushState()
		{
			return m_bMouseDown;
		}
		virtual inline POINT GetMousePos()
		{
			return m_ptMousePos;
		}
		virtual inline bool GetPreesKeyboard()
		{
			return m_bCheckPress;
		}
	};
}

