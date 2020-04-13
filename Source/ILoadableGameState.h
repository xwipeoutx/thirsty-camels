#pragma once

#include "CGameManager.h"
#include "IGameState.h"
#include "common.h"
#include <string>

namespace TTC
{
	class ILoadableGameState : public IGameState
	{
	public:
		virtual ~ILoadableGameState()
		{
		}

		/**
		 * Loads the resource pass pass by pass. Return the %ge load complete
		 */
		virtual float PreLoadState(int pass) = 0;
		virtual int GetLoadPassesRequired() = 0;
	protected:
		ILoadableGameState()
		: mNextPass(0)
		{ }
		int mNextPass;
	};
}