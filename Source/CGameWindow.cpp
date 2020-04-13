#include "CGameWindow.h"
#include "CGameManager.h"
#include "CConfig.h"
#include "CTimer.h"
#include "common.h" 
#include <sstream>

using namespace TTC;
CGameWindow::CGameWindow(int width, int height, int bpp, bool fullscreen)
: mFPS(0)
{
	mWidth = width;
	mHeight = height;
	mBPP = bpp;
	Uint32 flags = SDL_OPENGL | SDL_GL_ACCELERATED_VISUAL;
	if (fullscreen) flags |= SDL_FULLSCREEN;

	mScreen = SDL_SetVideoMode(
		mWidth, mHeight, 
		mBPP, 
		flags);

	if (mScreen == NULL)
		throw(TTC::Exception(std::string("Error setting video mode - ") + SDL_GetError()));

	mWindowOK = true;
	std::string title = TTC_CFG_GET_STR("windowTitle");
	SDL_WM_SetCaption(title.c_str(), NULL);
	mWindowed = true;

	mFPSTimer.Start();
	mFrame = 0;
}

CGameWindow::~CGameWindow(void)
{
	SDL_FreeSurface(mScreen);
}

void CGameWindow::HandleEvent(const SDL_Event &e)
{
	if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_F9)
	{
		//this->ToggleFullScreen();
	}
	if (e.type == SDL_QUIT)
		CGameManager::GetSingleton().Shutdown();
}
/*
void CGameWindow::ToggleFullScreen()
{
	return;
	if (mWindowed)
	{
		mScreen = SDL_SetVideoMode(mWidth, mHeight, mBPP, SDL_OPENGL | SDL_FULLSCREEN | SDL_GL_ACCELERATED_VISUAL);
		if (mScreen == NULL)
		{
			mWindowOK = false;
			return;
		}
		mWindowed = false;
	}
	else
	{	
		mScreen = SDL_SetVideoMode(mWidth, mHeight, mBPP, SDL_OPENGL | SDL_GL_ACCELERATED_VISUAL);

		if (mScreen == NULL)
		{
			mWindowOK = false;
			return;
		}
		mWindowed = true;
	}
}
*/

void CGameWindow::StartFrame()
{
	glClear(GL_COLOR_BUFFER_BIT);	
}

void CGameWindow::EndFrame()
{
	//glFinish();
	SDL_GL_SwapBuffers();

	//framerate
	mFrame++;
	if (mFPSTimer.GetTicks() > 1000)
	{
		float mFPS = 1000.0f*mFrame/mFPSTimer.GetTicks();
		mFrame = 0;
		mFPSTimer.Start();

		//put in window
		std::ostringstream strFPS;
		std::string title = TTC_CFG_GET_STR("windowTitle");
		strFPS << title << " - FPS: " << mFPS;
		SDL_WM_SetCaption(strFPS.str().c_str(), NULL);

	}
}