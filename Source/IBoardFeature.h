#pragma once

#include "common.h"

namespace TTC
{
	class CGameBoard;

	class IBoardFeature
	{
	public:
		IBoardFeature(){};
		~IBoardFeature(){};

		virtual bool OnAttach(CGameBoard *gameBoard)
		{
			mGameBoard = gameBoard;
			return true;
		}

		virtual void HandleEvent(const SDL_Event &e) {}
		virtual bool Update(float dt)=0;
		virtual void Draw()=0;

	protected:
		CGameBoard *GetGameBoard() { return mGameBoard; }
	private:
		CGameBoard *mGameBoard;
	};
}