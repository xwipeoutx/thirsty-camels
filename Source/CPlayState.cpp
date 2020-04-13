#include "CPlayState.h"
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

#include "CCamel.h"

#include "CRandomInjector.h"
#include "CScorezone.h"

#include <iostream>

using namespace TTC;
using namespace CEGUI;
using namespace FluidSolver;
using namespace std;

CPlayState::CPlayState(void)
: mElapsed(0), mWindow(NULL), mGameBoard(NULL), 
mBallManager(NULL), mRandomInjector(NULL)
{
}

CPlayState::~CPlayState(void)
{
	Exit();
}

float CPlayState::PreLoadState(int pass)
{
	if (pass == 0) 
	{
		//load the play layout
		mWindow = WindowManager::getSingleton().loadWindowLayout( "playlayout.xml" );
	} 
	else if (pass == 1)
	{
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
		float scale = 0.25f;
		mGameBoard->Attach(mCamel1 = new CCamel(FluidSolver::Vector2(scale*4, Height/2), -1, scale, SDLK_s));
		mGameBoard->Attach(mCamel2 = new CCamel(FluidSolver::Vector2(Width-scale*4, Height/2), 1, scale, SDLK_k));
	}
	else if (pass == 4)
	{
		glEnable(GL_TEXTURE_2D);
	}
	mNextPass++;
	return (pass + 1.0f) / GetLoadPassesRequired();
}

void CPlayState::Enter()
{
	//not entered through PreLoadState, load it ourselves
	for (int i=mNextPass; i < GetLoadPassesRequired(); i++) PreLoadState(i);
	System::getSingleton().setGUISheet( mWindow );
}

void CPlayState::Exit()
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
	if (mCamel1 != NULL) 
	{
		mGameBoard->Detach(mCamel1);
		delete mCamel1;
		mCamel1 = NULL;
	}
	if (mCamel2 != NULL) 
	{
		mGameBoard->Detach(mCamel2);
		delete mCamel2;
		mCamel2 = NULL;
	}
	
	if (mGameBoard != NULL) delete mGameBoard;
	mGameBoard = NULL;

	// Physics Manager MUST be destroyed AFTER the game board.
	delete CPhysicsManager::GetSingletonPtr();

	// Destroy CEGUI stuff
	WindowManager::getSingleton().destroyWindow("Play");
	System::getSingleton().setGUISheet( NULL );
}

void CPlayState::Pause()
{

}

void CPlayState::Resume()
{
}

void CPlayState::HandleEvent(const SDL_Event &e)
{
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
		else if (e.key.keysym.sym == SDLK_g) 
		{
			mGameBoard->GetFluid()->InjectCheckeredData();
		}
	}

}
void CPlayState::Draw()
{
	mGameBoard->Draw();
	
	/*
	// score
	static int mLastScore = 0;
	if (mLastScore != mGameBoard->GetScore())
	{
		mLastScore = mGameBoard->GetScore();
		std::string score = "Score: " + ToString( mLastScore );
		WindowManager::getSingleton().getWindow("Play/HUD/Score1")->setText(score);
	}
	*/

}
bool CPlayState::Update(float dt)
{
	if ((int)mElapsed != (int)(mElapsed + dt))
	{
		//spawn a new food every second
		SpawnRandomBall();
		std::string elapsed = "Time: " + ToString( (int)(mElapsed + dt) );
		WindowManager::getSingleton().getWindow("Play/HUD/Timer")->setText(elapsed);
	}
	mElapsed += dt;

	// FIXME Cache scores for performance++
	std::string score = "P1: " + ToString( mCamel1->GetScore() );
	WindowManager::getSingleton().getWindow("Play/HUD/Score1")->setText(score);
	score = "P2: " + ToString( mCamel2->GetScore() );
	WindowManager::getSingleton().getWindow("Play/HUD/Score2")->setText(score);


	mGameBoard->Update(dt);

	return true;
}
void CPlayState::SpawnRandomBall()
{
	mBallManager->AddBall(randf()*Width/4+ 3*Width/8, randf()*Height/4 + 3*Height/8);
}