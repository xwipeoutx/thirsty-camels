#pragma once

#include "CGameManager.h"
#include "common.h"
#include <string>

namespace TTC
{
	class IGameState
	{
	public:
		virtual ~IGameState(){
		}

		virtual void Enter() = 0;
		virtual void Exit() = 0;
		virtual void Pause() = 0;
		virtual void Resume() = 0;

		virtual void HandleEvent(const SDL_Event &e){}
		virtual void Draw(){}
		virtual bool Update(float dt){return true;}

		void ChangeState(TTC::IGameState* state) { CGameManager::GetSingletonPtr()->ChangeState(state); }
		void PushState(TTC::IGameState* state) { CGameManager::GetSingletonPtr()->PushState(state); }
		void PopState() { CGameManager::GetSingletonPtr()->PopState(); }

		inline virtual const std::string GetStateName()=0;
	protected:
		IGameState() { }
	};
}