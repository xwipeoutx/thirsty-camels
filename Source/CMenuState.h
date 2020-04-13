#pragma once

#include "CEGUI.h"

#include "ILoadableGameState.h"
#include "CSprite.h"

namespace TTC
{
	class COptionsWindow;
	class CGameBoard;
	class CRandomInjector;

	class CMenuState : public ILoadableGameState
	{
	public:
		CMenuState(void);
		~CMenuState(void);
		
		float PreLoadState(int pass);
		int GetLoadPassesRequired();

		virtual void Enter();
		virtual void Exit();

		virtual void Pause();
		virtual void Resume();
		
		virtual void HandleEvent(const SDL_Event &e);
		virtual bool Update(float dt);
		virtual void Draw();
		
		inline virtual const std::string GetStateName(){return "MenuState";}
		bool onFreePlayButtonClicked(const CEGUI::EventArgs &e);
		bool onPlayButtonClicked(const CEGUI::EventArgs &e);
		bool onOptionsButtonClicked(const CEGUI::EventArgs &e);
		bool onQuitButtonClicked(const CEGUI::EventArgs &e);

	protected:
		CEGUI::Window *mWindow;
		COptionsWindow *mOptionsWindow;
		
		CRandomInjector *mRandomInjector;
		CGameBoard *mGameBoard;
	};
}