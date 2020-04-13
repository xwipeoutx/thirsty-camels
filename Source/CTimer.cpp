#include "CTimer.h"

#include "common.h"

using namespace TTC;

CTimer::CTimer()
{
	mStartTicks = mPausedTicks = 0;
	mPaused = mStarted = 0;
}

CTimer::~CTimer(void)
{
}

void CTimer::Start()
{
	mStarted = true;
	mPaused = false;
	mStartTicks = SDL_GetTicks();
}
void CTimer::Stop()
{
	mStarted = mPaused = false;
}
int CTimer::GetTicks()
{
	if (mStarted)
	{
		if (mPaused)
		{
			return mPausedTicks;
		}
		else
		{
			return SDL_GetTicks() - mStartTicks;
		}
	}
	return 0;
}
void CTimer::Pause()
{
	if (mStarted && !mPaused)
	{
		mPaused = true;
		mPausedTicks = SDL_GetTicks() - mStartTicks;
	}
}

void CTimer::Resume()
{
	if (mPaused)
	{
		mPaused = false;
		mStartTicks = SDL_GetTicks() - mPausedTicks;
		mPausedTicks = 0;
	}
}
