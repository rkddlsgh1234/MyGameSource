#pragma once
#include <Windows.h>
namespace Engine
{
	class TimeManager
	{
	private:
		static TimeManager* m_Instance;

		bool				m_bUseFPS;
		int					m_nFPS;
		DWORD				m_dwLastTime;
		DWORD				m_dwCurTime;
		float				m_fElapseTime;

		TimeManager();
		~TimeManager();
	public:
		static TimeManager* GetInstance();

		void Init(bool useFPS, int FPS);
		bool Update();
		void Release();
		virtual inline float GetElipseTime()
		{
			return m_fElapseTime;
		}
	};
}

