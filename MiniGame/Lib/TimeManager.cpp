#include "TimeManager.h"
#include "IncludeDefine.h"

namespace Engine
{

	TimeManager::TimeManager()
	{
		m_bUseFPS = false;
		m_nFPS = 5;

		m_dwLastTime = GetTickCount();
		m_dwCurTime = GetTickCount();
	}


	TimeManager::~TimeManager()
	{
	}
	TimeManager* TimeManager::m_Instance = NULL;

	TimeManager* TimeManager::GetInstance()
	{
		if (m_Instance == NULL)
		{
			m_Instance = new TimeManager;
		}

		return m_Instance;
	}

	void TimeManager::Init(bool useFPS, int FPS)
	{
		m_bUseFPS = useFPS;
		m_nFPS = FPS;
	}

	bool TimeManager::Update()
	{
		m_dwCurTime = GetTickCount();

		if (m_bUseFPS)
		{
			if (m_dwCurTime - m_dwLastTime < 1000 / m_nFPS)
				return false;
		}


		m_fElapseTime = (m_dwCurTime - m_dwLastTime) / 1000.0f;
		m_dwLastTime = m_dwCurTime;

		return true;
	}
	void TimeManager::Release()
	{
		SAFE_DELETE(m_Instance);
	}
}
