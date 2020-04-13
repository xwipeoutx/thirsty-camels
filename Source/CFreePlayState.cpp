#include "CFreePlayState.h"
#include "COptionsWindow.h"
#include "CMenuState.h"
#include "CGameManager.h"
#include "CGraphicsManager.h"
#include "CHud.h"
#include "CConfig.h"
#include "CEGUI.h"
#include "Exception.h"
#include "CLoadState.h"
#include "CGameBoard.h"
#include "CBallManager.h"
#include "CFluidInjector.h"

#include "CRandomInjector.h"
#include "CScorezone.h"

#include <iostream>

using namespace TTC;
using namespace CEGUI;
using namespace std;

CFreePlayState::CFreePlayState(void)
: mElapsed(0), mWindow(NULL), mOptionsWindow(NULL), mAutoInject(false), mGameBoard(NULL), 
mBallManager(NULL), mScorezone(NULL), mRandomInjector(NULL), mFluidInjector(NULL)
{
}

CFreePlayState::~CFreePlayState(void)
{
	Exit();
}

float CFreePlayState::PreLoadState(int pass)
{
	if (pass == 0) 
	{
		//load the play layout
		mWindow = WindowManager::getSingleton().loadWindowLayout( "freeplaylayout.xml" );
	} 
	else if (pass == 1)
	{
		//load options window
		mOptionsWindow = new COptionsWindow(false, mWindow);
	} 
	else if (pass == 2)
	{
		new CPhysicsManager();
		Box bounds;
		bounds.x = bounds.y = 0;
		bounds.w = Width;
		bounds.h = Height;
		CPhysicsManager::GetSingleton().SetupWorld(bounds);
	}
	else if (pass == 3)
	{
		mGameBoard = new CGameBoard();
		mGameBoard->Initialize(Width, Height);

		mGameBoard->Attach(mBallManager = new CBallManager());
		mGameBoard->Attach(mScorezone = new CScorezone());
		mGameBoard->Attach(mFluidInjector = new CFluidInjector());
	}
	else if (pass == 4)
	{
		glEnable(GL_TEXTURE_2D);
	}
	mNextPass++;
	return (pass + 1.0f) / GetLoadPassesRequired();
}

void CFreePlayState::Enter()
{
	//not entered through PreLoadState, load it ourselves
	for (int i=mNextPass; i < GetLoadPassesRequired(); i++) PreLoadState(i);
	System::getSingleton().setGUISheet( mWindow );
}

void CFreePlayState::Exit()
{
	if (mBallManager != NULL) 
	{
		mGameBoard->Detach(mBallManager);
		delete mBallManager;
		mBallManager = NULL;
	}
	if (mRandomInjector != NULL) 
	{
		mGameBoard->Detach(mRandomInjector);
		delete mRandomInjector;
		mRandomInjector = NULL;
	}
	if (mFluidInjector != NULL) 
	{
		mGameBoard->Detach(mFluidInjector);
		delete mFluidInjector;
		mFluidInjector = NULL;
	}
	if (mScorezone != NULL) 
	{
		mGameBoard->Detach(mScorezone);
		delete mScorezone;
		mScorezone = NULL;
	}
	
	if (mGameBoard != NULL) delete mGameBoard;
	mGameBoard = NULL;

	// Physics Manager MUST be destroyed AFTER the game board.
	delete CPhysicsManager::GetSingletonPtr();

	if (mOptionsWindow != NULL) delete mOptionsWindow;
	mOptionsWindow = NULL;

	// Destroy CEGUI stuff
	WindowManager::getSingleton().destroyWindow("FreePlay");
	System::getSingleton().setGUISheet( NULL );
}

void CFreePlayState::Pause()
{

}

void CFreePlayState::Resume()
{
}

void CFreePlayState::HandleEvent(const SDL_Event &e)
{
	if (mOptionsWindow->IsActive()) return;

	mGameBoard->HandleEvent(e);

	if (e.type == SDL_KEYDOWN)
	{
		//escape - back to menu screen
		if (e.key.keysym.sym == SDLK_ESCAPE) 
		{
			CGameManager::GetSingleton().ChangeState(new CLoadState(new CMenuState()));
		}
		else if (e.key.keysym.sym == SDLK_d) 
		{
			CPhysicsManager::GetSingleton().ToggleDebug();
		}
		else if (e.key.keysym.sym == SDLK_o) 
		{
			mOptionsWindow->Show();
		}
		else if (e.key.keysym.sym == SDLK_g) 
		{
			mGameBoard->GetFluid()->InjectCheckeredData();
		}
		else if (e.key.keysym.sym == SDLK_j) 
		{
			mAutoInject = !mAutoInject;
			
			if (mAutoInject)
			{
				mGameBoard->Attach( mRandomInjector = new CRandomInjector() );
			} else if (mRandomInjector != NULL) {

				mGameBoard->Detach(mRandomInjector);
				delete mRandomInjector;
				mRandomInjector = NULL;
			}
		}
		else if (e.key.keysym.sym == SDLK_SPACE)
		{
			mBallManager->AddBall(randf()*Width, randf()*Height);
		}
	}

}
void CFreePlayState::Draw()
{
	mGameBoard->Draw();

	// score
	static int mLastScore = 0;
	if (mLastScore != mGameBoard->GetScore())
	{
		mLastScore = mGameBoard->GetScore();
		std::string score = "Score: " + ToString( mLastScore );
		WindowManager::getSingleton().getWindow("FreePlay/HUD/Score")->setText(score);
	}

}
bool CFreePlayState::Update(float dt)
{
	if (mOptionsWindow->IsActive())
	{
		if (mOptionsWindow->IsDone())
		{
			mOptionsWindow->Hide();
			if (mOptionsWindow->IsApplied()) mGameBoard->Reset(true);
		}
	}
	else
	{
		mElapsed += dt;

		//options label
		Window *window = WindowManager::getSingleton().getWindow("FreePlay/HUD/OptionsLabel");
		if (mElapsed > 5 && mElapsed < 10)
		{
			window->setAlpha(window->getAlpha() - 0.2f*dt);
		}
		else if (mElapsed > 10) 
		{
			if (window->isVisible()) window->hide();
		}

		// Game board update
		mGameBoard->Update(dt);
	}

	return true;
}