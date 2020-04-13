#include "CMenuState.h"

#include "CGameManager.h"
#include "CHud.h"
#include "CLoadState.h"
#include "COptionsWindow.h"
#include "CFreePlayState.h"
#include "CPlayState.h"
#include "CGUIManager.h"

#include "CGameBoard.h"
#include "CRandomInjector.h"

using namespace TTC;
using namespace CEGUI;
CMenuState::CMenuState(void)
: mWindow(NULL), mGameBoard(NULL), mRandomInjector(NULL)
{
	
}

CMenuState::~CMenuState(void)
{
	
}

int CMenuState::GetLoadPassesRequired()
{
	return 4;
}

float CMenuState::PreLoadState(int pass)
{
	float percentDone = 0;
	if (pass == 0) 
	{
		//load the menu layout
		WindowManager &windowManager = WindowManager::getSingleton();
		mWindow = WindowManager::getSingleton().loadWindowLayout( "menulayout.xml" );
		percentDone = 60;
	} 
	else if (pass == 1)
	{
		//load options window
		mOptionsWindow = new COptionsWindow(true, mWindow);
		percentDone = 75;
	} 
	else if (pass == 2)
	{	//subscribe events for play and quit buttons
		Window *childWindow = NULL;
		
		childWindow = WindowManager::getSingleton().getWindow("Menu/FreePlayButton");
		childWindow->subscribeEvent(PushButton::EventClicked, Event::Subscriber(&CMenuState::onFreePlayButtonClicked, this));
		childWindow = WindowManager::getSingleton().getWindow("Menu/PlayButton");
		childWindow->subscribeEvent(PushButton::EventClicked, Event::Subscriber(&CMenuState::onPlayButtonClicked, this));
		childWindow = WindowManager::getSingleton().getWindow("Menu/OptionsButton");
		childWindow->subscribeEvent(PushButton::EventClicked, Event::Subscriber(&CMenuState::onOptionsButtonClicked, this));
		childWindow = WindowManager::getSingleton().getWindow("Menu/QuitButton");
		childWindow->subscribeEvent(PushButton::EventClicked, Event::Subscriber(&CMenuState::onQuitButtonClicked, this));

		childWindow = WindowManager::getSingleton().getWindow("Menu/HighScores");
		childWindow->setText("FIXME: Scores");
		percentDone = 85;
	}
	else if (pass == 3)
	{
		mGameBoard = new CGameBoard();
		mGameBoard->Initialize(8, 6);
		mGameBoard->Attach(mRandomInjector = new CRandomInjector());
		percentDone = 100;
	}
	mNextPass++;
	return percentDone;
}

void CMenuState::Enter()
{
	CLog::Get().Info("Entered Menu State");
	for (int i=mNextPass; i < GetLoadPassesRequired(); i++) PreLoadState(i);
	System::getSingleton().setGUISheet( mWindow );
}

void CMenuState::Exit()
{
	if (mRandomInjector != NULL) {
		mGameBoard->Detach(mRandomInjector);
		delete mRandomInjector;
		mRandomInjector = NULL;
	}

	if (mGameBoard != NULL) delete mGameBoard;
	mGameBoard = NULL;

	delete mOptionsWindow;
	WindowManager::getSingleton().destroyWindow("Menu");
	System::getSingleton().setGUISheet( NULL );
}

void CMenuState::Pause()
{
}

void CMenuState::Resume()
{
}

bool CMenuState::onFreePlayButtonClicked(const CEGUI::EventArgs &e)
{
	CGameManager::GetSingleton().ChangeState(new CLoadState(new CFreePlayState()));
	return true;
}

bool CMenuState::onPlayButtonClicked(const CEGUI::EventArgs &e)
{
	CGameManager::GetSingleton().ChangeState(new CLoadState(new CPlayState()));
	return true;
}

bool CMenuState::onOptionsButtonClicked(const CEGUI::EventArgs &e)
{
	mOptionsWindow->Show();
	return true;
}

bool CMenuState::onQuitButtonClicked(const CEGUI::EventArgs &e)
{
	CGameManager::GetSingleton().Shutdown();
	return true;
}

void CMenuState::HandleEvent(const SDL_Event &e)
{
	if (mOptionsWindow->IsActive())
	{
		if (mOptionsWindow->IsDone()) 
		{
			mOptionsWindow->Hide();
		}
	} 
	else
	{
		if (e.type == SDL_KEYDOWN)
		{
			switch (e.key.keysym.sym)
			{
			case SDLK_RETURN:
				CGameManager::GetSingleton().ChangeState(new CLoadState(new CPlayState()));
				break;
			case SDLK_SPACE:
				CGameManager::GetSingleton().ChangeState(new CLoadState(new CFreePlayState()));
				break;
			case SDLK_ESCAPE:
				CGameManager::GetSingleton().Shutdown();
			default:
				break;
			}
		}
	}
}

bool CMenuState::Update(float dt)
{
	// Game board update
	mGameBoard->Update(dt);
	return true;
}

void CMenuState::Draw()
{
	mGameBoard->Draw();
}