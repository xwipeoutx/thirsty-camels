#pragma once

#include "IGameState.h"
#include "ILoadableGameState.h"

namespace TTC
{
	class CLoadState : public IGameState
	{
	public:
		CLoadState(ILoadableGameState *nextGameState);
		~CLoadState(void);

		virtual void Enter();
		virtual void Exit();

		virtual void Pause();
		virtual void Resume();
		
		virtual bool Update(float dt);
		virtual void Draw();
		
		inline virtual const std::string GetStateName(){return "LoadState";}
	private:
		ILoadableGameState *mNextGameState;
		int mLoadCounter;

		CEGUI::Window *mLoadingWrapper;
		CEGUI::Window *mLoadingFluid;
	};
}