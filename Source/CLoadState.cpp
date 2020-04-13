#include "CLoadState.h"
#include "CMenuState.h"
#include "CHud.h"
#include "CEGUI.h"
#include "utils.h"

using namespace TTC;
using namespace CEGUI;

// FIXME CACHE THIS WINDOW YO!

CLoadState::CLoadState(ILoadableGameState *nextGameState)
: mLoadCounter(-1), mNextGameState(nextGameState), mLoadingWrapper(NULL), mLoadingFluid(NULL)
{
}

CLoadState::~CLoadState(void)
{
	mNextGameState = NULL;
}

void CLoadState::Enter()
{
	//load the gui layout
	Window* window = WindowManager::getSingleton().loadWindowLayout( "loadlayout.xml" );
	System::getSingleton().setGUISheet( window );

	mLoadingWrapper = WindowManager::getSingleton().getWindow("Load/ProgressWrapper");
	mLoadingFluid = WindowManager::getSingleton().getWindow("Load/ProgressFluid");
	
}

void CLoadState::Exit()
{
	//destroy CEGUI stuff
	WindowManager::getSingleton().destroyWindow("Load");
	System::getSingleton().setGUISheet( NULL );
}

void CLoadState::Pause()
{
}

void CLoadState::Resume()
{
}

bool CLoadState::Update(float dt)
{
	int passesRequired = mNextGameState->GetLoadPassesRequired();
	if (mLoadCounter < passesRequired) 
	{
		float loadPercent = 0;
		if (mLoadCounter >= 0) loadPercent = mNextGameState->PreLoadState(mLoadCounter);
		else ; //first pass - just skip it, so we get a load screen ASAP
		mLoadCounter++;

		mLoadingWrapper->setText
		(
			ToString(mLoadCounter) + "/" + ToString(passesRequired)
		);
		
		mLoadingFluid->setWidth(UDim(loadPercent, 0));
	}
	else
	{
		//load the next game state - give it 2 frames of grace, because we're so nice
		CGameManager::GetSingleton().PopState();
		CGameManager::GetSingleton().PushState(mNextGameState);
	}


	return true;
}

void CLoadState::Draw()
{
}
