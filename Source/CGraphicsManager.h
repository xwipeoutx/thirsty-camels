#pragma once

#include "CSingleton.h"
#include "common.h"

namespace TTC
{
	class CGraphicsManager : public CSingleton<CGraphicsManager>
	{
	public:
		CGraphicsManager();
		~CGraphicsManager();

		void Init();

		CGcontext GetCgContext() { return mCgContext; }
		CGprofile GetCgFragmentProfile() {return mCgFragmentProfile; }

	protected:
		void InitGL();
		void InitCg();

		CGcontext mCgContext;
		CGprofile mCgFragmentProfile;

	};
}