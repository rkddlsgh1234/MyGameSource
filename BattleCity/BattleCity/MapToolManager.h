#pragma once
#include <Windows.h>
#include <vector>
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }

using namespace std;

class ResManager;
class Tile;
class MapToolManager
{
private:
	static MapToolManager* m_Instance;

	HDC		m_hMemDC[2];
	HBITMAP	m_hBitMap[2];
	HBITMAP	m_hOldBitMap[2];
	HWND hWnd;

	int m_iSelectState;

	ResManager* m_pResource;
	vector <Tile*> m_vcTile;
	vector <Tile*> m_vcLoad;
	vector <Tile*> m_vcMapTool;

	bool m_bCheckDrawMode;

	MapToolManager();
	~MapToolManager();
public:
	static MapToolManager* GetInstance();

	void Init(HWND hWnd, HINSTANCE hInst);
	void Draw();
	void Updata();
	void Release();

	void DrawTile(int x, int y);
	void ReMoveTile(int x, int y);
	void SetDrawTool(int x, int y);
	void SetDrowMode();

	void SaveFile();
	void LoadFile();


	virtual inline int GetTileState()
	{
		return m_iSelectState;
	}
	virtual inline int GetCheckDrawMode()
	{
		return m_bCheckDrawMode;
	}
};

