#pragma once

#include "ILoadableGameState.h"
#include "common.h"
#include "FluidSolver.h"

namespace TTC
{
	class COptionsWindow;
	class CBall;

	class CFreePlayState : public ILoadableGameState
	{
	public:
		CFreePlayState(void);
		~CFreePlayState(void);

		float PreLoadState(int pass);
		int GetLoadPassesRequired() { return 5; }

		virtual void Enter();
		virtual void Exit();

		virtual void Pause();
		virtual void Resume();
		
		virtual void HandleEvent(const SDL_Event &e);
		virtual void Draw();
		virtual bool Update(float dt);
		
		inline virtual const std::string GetStateName(){return "FreePlayState";}

	private:
		static const int Width = 8;
		static const int Height = 6;
		FluidSolver::Fluid *mFluid;
		FluidSolver::FluidOptions mOptions;
		float mElapsed;
		bool mAutoInject;
		
		typedef std::list<CBall *> BallList;
		BallList mBalls;

		CEGUI::Window *mWindow;
		COptionsWindow *mOptionsWindow;

		void SetFluidOptions();

	};
}