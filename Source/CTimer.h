#pragma once

namespace TTC
{
	class CTimer
	{
	public:
		CTimer();
		~CTimer();
		void Start();
		void Stop();
		void Pause();
		void Resume();
		int GetTicks();
		bool IsStarted(){return mStarted;}
		bool IsPaused(){return mPaused;}
	private:
		int mStartTicks;
		int mPausedTicks;

		bool mPaused;
		bool mStarted;
	};
}

class Timer
{
public:

};

