#include "ResManager.h"


namespace Engine
{
	ResManager::ResManager()
	{
		FilePath = "..\\Res\\";
	}


	ResManager::~ResManager()
	{
	}
	ResManager* ResManager::m_Instance = NULL;

	ResManager* ResManager::GetInstance()
	{
		if (m_Instance == NULL)
		{
			m_Instance = new ResManager;
		}

		return m_Instance;
	}

	void ResManager::InitBack(HDC hdc, int WinSizeX, int WinSizeY)
	{
		m_pBack = new Engine::BitMap;
		m_pBack->InitBack(hdc, WinSizeX, WinSizeY);
	}
	
	Engine::BitMap* ResManager::InitBitMap(string FileName)
	{
		Engine::BitMap* pBitMap = new Engine::BitMap;

		pBitMap->Init(m_pBack->ReturnDC(), FileName);
		m_vcBitMap.push_back(pBitMap);

		return pBitMap;
	}
	Engine::BitMap* ResManager::GetBitMap(string FileName)
	{
		for (auto i = m_vcBitMap.begin(); i != m_vcBitMap.end(); i++)
		{
			if ((*i)->GetFileName() == FileName)
				return (*i);
		}
		Engine::BitMap* pBitMap = new Engine::BitMap;
		pBitMap->Init(m_pBack->ReturnDC(), FileName);
		m_vcBitMap.push_back(pBitMap);

		return pBitMap;
	}
	
	string ResManager::ReturnFileDirectory(string FileName)
	{
		string Result = FilePath + FileName;

		return Result;
	}

	HDC ResManager::ReturnBackDC()
	{
		return m_pBack->ReturnDC();
	}

	void ResManager::DrawBack(HDC hdc)
	{
		m_pBack->DrawBack(hdc);
	}
	void ResManager::DrawBitMap(HDC hdc, string FileName)
	{

	}

	void ResManager::Release()
	{
		SAFE_DELETE(m_pBack);
		for (auto i = m_vcBitMap.begin(); i != m_vcBitMap.end(); i++)
		{
			SAFE_DELETE((*i));
		}

		SAFE_DELETE(m_Instance);
	}
}