#include "CGameManager.h"
#include "IGameState.h"

#include "CGameWindow.h"
#include "CGraphicsManager.h"

#include "CTimer.h"
#include "CGameWindow.h"
#include "CHud.h"
#include "CConfig.h"

#include "common.h"
using namespace TTC;

CGameManager::CGameManager()
: mRunning(true), mGUIManager(0), mRestart(false)
{
	new CLog();
	new CHud();
	new CConfig();
	mGUIManager = new CGUIManager();
}

CGameManager::~CGameManager()
{
	Mix_CloseAudio();
	if (CHud::IsValid())
		delete CHud::GetSingletonPtr();
	if (mGUIManager != NULL)
	{
		mGUIManager->Destroy();
		delete mGUIManager;
		mGUIManager = NULL;
	}
	if (CLog::IsValid())
		delete CLog::GetSingletonPtr();
	if (CGameWindow::IsValid())
		delete CGameWindow::GetSingletonPtr();
	if (CConfig::IsValid())
		delete CConfig::GetSingletonPtr();
}

void CGameManager::Start(IGameState* state)
{
	CLog::Get().Info("Starting Game Manager");
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
		throw(Exception(SDL_GetError()));

	CConfig::GetSingleton().Load("config.ini");
	CConfig::GetSingleton().Save("config.ini"); //fixme

	//set log level from config file
	const std::string &logLevel = TTC_CFG_GET_STR("logLevel");
	CLog::Get().SetLogLevel(logLevel);

	int resX = TTC_CFG_GET(int, "resolutionX");
	int resY = TTC_CFG_GET(int, "resolutionY");
	
	new CGameWindow(resX, resY, 32, false);
	new CGraphicsManager();
	CGraphicsManager::GetSingleton().Init();


	//FIXME: Audio Options
	mGUIManager->Init();
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 8) == -1 )
		throw Exception(std::string("Error initialising audio: ") + Mix_GetError());

	CLog::Get().Info("Sound Mixer Initialised");
	CLog::Get().Info("Build Stamp: " + GetBuildStamp());

	CLog::Get().Debug("Game Manager Started");
	ChangeState(state);
}

void CGameManager::Stop()
{	
	delete CGraphicsManager::GetSingletonPtr();

	// clean up all the states
	while (!mStates.empty()) {
		mStates.back()->Exit();
		mStates.pop_back();
	}
	Mix_CloseAudio();
}

bool CGameManager::Run()
{
	CGameWindow *window = CGameWindow::GetSingletonPtr();

	CTimer deltaTimer;
	deltaTimer.Start();

	CLog::Get().Info("Entered Game Loop");
	while (mRunning)
	{
		SDL_Event e;

		//handle any events
		while (SDL_PollEvent(&e))
		{

			CLog::Get().Debug("Received Event...");
			if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_m)
			{
				static int prevVolume = 0;
				prevVolume = Mix_Volume(-1, prevVolume);
				if (Mix_PausedMusic()) Mix_ResumeMusic();
				else Mix_PauseMusic();
			}
			window->HandleEvent(e);
			mGUIManager->HandleEvent(e);
			mStates.back()->HandleEvent(e);
			CLog::Get().Debug("Event Handled...");
		}
		
		//update timer
		int numTicks = deltaTimer.GetTicks();
		deltaTimer.Start();
		float dt = 0.001f * numTicks;
		
		//update window and current state
		CLog::Get().Debug("Updating... dt=" + TTC::ToString(dt));
		mStates.back()->Update(dt);
		window->Update(dt);
		mGUIManager->Update(dt);
		mRunning = mRunning;// & mStates.back()->Update(dt);

		//draw
		window->StartFrame();
		CLog::Get().Debug("Started Frame");
		mStates.back()->Draw();
		CLog::Get().Debug("Finished drawing state");
		mGUIManager->Draw();
		CLog::Get().Debug("Finished drawing GUI");
		window->EndFrame();
	}
	CLog::Get().Info("Left Game Loop");
	return mRestart;
}

void CGameManager::Restart()
{
	CLog::Get().Info("Game Manager Restart");
	mRestart = true;
	mRunning = false;
}

void CGameManager::Shutdown()
{
	CLog::Get().Info("Game Manager Shutdown");
	mRunning = false;
}

void CGameManager::ChangeState(IGameState *state)
{
	CLog::Get().Debug("Changing State to: " + state->GetStateName());
	// clean up all the current states
	while ( !mStates.empty() ) {
		CLog::Get().Debug("|- Clearing State: " + mStates.back()->GetStateName());
		mStates.back()->Exit();
		mStates.pop_back();
	}

	// store and init the new state
	mStates.push_back(state);
	mStates.back()->Enter();
}

void CGameManager::PushState(IGameState* state)
{
	CLog::Get().Debug("Pushing State: " + state->GetStateName());
	// pause current state
	if ( !mStates.empty() ) {
		CLog::Get().Debug("|- Pausing State: " + mStates.back()->GetStateName());
		mStates.back()->Pause();
	}

	// store and init the new state
	mStates.push_back(state);
	mStates.back()->Enter();
}

void CGameManager::PopState()
{
	// cleanup the current state
	if ( !mStates.empty() ) {
		CLog::Get().Debug("Popping State: " + mStates.back()->GetStateName());
		mStates.back()->Exit();
		mStates.pop_back();
	}

	// resume previous state
	if ( !mStates.empty() ) {
		CLog::Get().Debug("|- Current State: " + mStates.back()->GetStateName());
		mStates.back()->Resume();
	}
}