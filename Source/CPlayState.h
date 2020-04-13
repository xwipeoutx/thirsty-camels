#pragma once

#include "ILoadableGameState.h"
#include "common.h"
#include "FluidSolver.h"

namespace TTC
{
	class COptionsWindow;
	class CBall;
	class CGameBoard;
	class CBallManager;
	class CScorezone;
	class CRandomInjector;
	class CFluidInjector;
	class CCamel;

	class CPlayState : public ILoadableGameState
	{
	public:
		CPlayState(void);
		~CPlayState(void);

		float PreLoadState(int pass);
		int GetLoadPassesRequired() { return 5; }

		virtual void Enter();
		virtual void Exit();

		virtual void Pause();
		virtual void Resume();
		
		virtual void HandleEvent(const SDL_Event &e);
		virtual void Draw();
		virtual bool Update(float dt);
		
		inline virtual const std::string GetStateName(){return "PlayState";}

	protected:
		void SpawnRandomBall();
	private:
		// FIXME: Config?
		static const int Width = 8;
		static const int Height = 6;

		CGameBoard *mGameBoard;
		float mElapsed;
		
		CEGUI::Window *mWindow;
		
		// Attachments
		CBallManager *mBallManager;
		CRandomInjector *mRandomInjector;

		CCamel *mCamel1;
		CCamel *mCamel2;
	};
}