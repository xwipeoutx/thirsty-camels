#include "CGraphicsManager.h"


using namespace TTC;

CGraphicsManager::CGraphicsManager()
{
}

CGraphicsManager::~CGraphicsManager()
{
}


void CGraphicsManager::Init()
{
	InitGL();
	InitCg();
}

void CGraphicsManager::InitGL()
{
	glClearColor(0.0,0.0,0.0,0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 1, 0, 1, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//enable stuff/settings
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (glGetError() != GL_NO_ERROR)
		throw  Exception("Error initialising OpenGL");

	//FIXME: Errors
	glewInit();
}

void CGraphicsManager::InitCg()
{
	//fixme cgSetErrorCallback(CGErrorCallback);
	mCgContext = cgCreateContext();
	mCgFragmentProfile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
	cgGLSetOptimalOptions(mCgFragmentProfile);
}

