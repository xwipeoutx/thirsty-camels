#pragma once

#include "IGameState.h"
#include "common.h"
#include "FluidSolver.h"

namespace TTC
{
	class COptionsWindow
	{
	public:
		COptionsWindow(bool enableResolution, CEGUI::Window *parentWindow);
		~COptionsWindow(void);

		void Init();
		
		virtual void Show();
		virtual void Hide();
		
		bool IsActive() { return mWindow->isVisible(); }
		bool IsDone() { return mDone; }
		bool IsApplied() { return mApplied; }

		inline virtual const std::string GetStateName(){return "OptionState";}
		
		bool onOkButtonClicked(const CEGUI::EventArgs &e);
		bool onCancelButtonClicked(const CEGUI::EventArgs &e);
	protected:
		bool mEnableResolution;

	private:
		const char* GetFromWindow(std::string key);
		bool mDone, mApplied;

		//option controls - here for a bit of caching and ease
		CEGUI::Window *mParentWindow;
		CEGUI::Window *mWindow;
		CEGUI::PushButton *mOkButton;
		CEGUI::PushButton *mCancelButton;
		CEGUI::Window *mResolutionLabel;
		CEGUI::Combobox *mResolution;
		CEGUI::Combobox *mFluidResolution;
		CEGUI::Combobox *mSolverQuality;
		CEGUI::Window *mViscosity;
		CEGUI::Window *mDensityR;
		CEGUI::Window *mDensityG;
		CEGUI::Window *mDensityB;


	};
}