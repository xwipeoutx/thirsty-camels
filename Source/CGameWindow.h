#pragma once

#include "CSingleton.h"
#include "CTimer.h"
#include "common.h" 

namespace TTC
{
	class CGameWindow : public CSingleton<CGameWindow>
	{
	public:
		CGameWindow(int width, int height, int bpp, bool fullscreen);
		~CGameWindow(void);
		
		void HandleEvent(const SDL_Event &event);
		bool Update(float dt){return true;}
		void StartFrame();
		void Resize(int width, int height);
		void EndFrame();


		//Accessors / Mutators
		bool IsWindowed(){return mWindowed;}
		//void ToggleFullScreen();
		const SDL_Surface *GetScreen(){return mScreen;}

		int GetWidth(){return mWidth;}
		int GetHeight(){return mHeight;}

		float getFPS(){return mFPS;}
		
	private:
		bool mWindowed;
		bool mWindowOK;
		SDL_Surface *mScreen;

		int mWidth;
		int mHeight;
		int mBPP;

		CTimer mFPSTimer;
		int mFrame;
		float mFPS;

	};
};