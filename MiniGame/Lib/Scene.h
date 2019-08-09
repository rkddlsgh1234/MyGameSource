#pragma once

enum GAME_STATE
{
	GAME_STATE_WAIT,
	GAME_STATE_RUN,
	GAME_STATE_TIMEOVER,
	GAME_STATE_END,
};

namespace Engine
{
	class Scene
	{
	public:
		Scene();
		virtual ~Scene();

		virtual void Init();
		virtual bool Input(float fElapseTime);
		virtual void Draw();
		virtual void Updata(float fElapseTime);
		virtual void Release();
	};
}

