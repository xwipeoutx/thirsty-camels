#pragma once

#include <vector>
#include "CSingleton.h"
#include "CGUIManager.h"

namespace TTC
{

	class CLog;
	class IGameState;

	class CGameManager : public CSingleton<CGameManager>
	{
	public:
		CGameManager();
		~CGameManager();
		void Start(IGameState* state);
		void Stop();
		
		bool Run();

		void Restart();
		void Shutdown();

		void ChangeState(IGameState* state);
		void PushState(IGameState* state);
		void PopState();

		const std::string GetBuildStamp(){return "20091113";}
	protected:		
		bool mRunning;
		bool mRestart;

		void SetupStates(void);
		void SetupPhysics(void);

	private:
		std::vector<IGameState*> mStates;
		
		CGUIManager *mGUIManager;
	};
}