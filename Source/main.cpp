#include "common.h" 

#include <fstream>

#include "CGameWindow.h"
#include "CGameManager.h"
#include "CMenuState.h"
#include "CLoadState.h"

#include "CEGUI.h"

using namespace TTC;

int main( int argc, char* argv[] )
{
	bool running = true;
	while (running) //fixme this is kinda ugly
	{
		CGameManager mgr;
		try
		{
			mgr.Start(new CLoadState(new CMenuState()));
			running = mgr.Run();
			mgr.Stop();
		} 
		catch (TTC::Exception ex)
		{
			CLog::GetSingleton().Fatal(ex.GetErrorString());
			running = false;
		}
#ifndef _DEBUG
		//fixme: catch any and all exceptions, just to stop it dying ungracefully.
		catch (...)
		{
			CLog::GetSingleton().Fatal("UNHANDLED EXCEPTION - CANNOT RECOVER!");
			running = false;
		}
#endif
	}

	return 0;
}
