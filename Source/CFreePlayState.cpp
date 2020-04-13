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
#include "CBall.h"

#include <iostream>
#include <ctime>

using namespace TTC;
using namespace CEGUI;
using namespace FluidSolver;
using namespace std;

CFreePlayState::CFreePlayState(void)
: mElapsed(0), mWindow(NULL), mOptionsWindow(NULL), mAutoInject(false)
{

}

CFreePlayState::~CFreePlayState(void)
{
	Exit();
}

float CFreePlayState::PreLoadState(int pass)
{
	srand((int)time(0));
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
		CGraphicsManager &graphicsMgr = CGraphicsManager::GetSingleton();
		mFluid = new FluidSolver::Fluid(
			graphicsMgr.GetCgContext(), 
			graphicsMgr.GetCgFragmentProfile(), 
			TTC_CFG_GET_STR("resourcePath") + "Shaders/Fluid/"
		);
		//Init fluid
		SetFluidOptions();
		mFluid->Init(mOptions);
		mFluid->SetColorDensities(TTC_CFG_GET(float, "densityR"), TTC_CFG_GET(float, "densityG"), TTC_CFG_GET(float, "densityB"));
	}
	else if (pass == 3)
	{
		new CPhysicsManager();
		Box bounds;
		bounds.x = bounds.y = 0;
		bounds.w = Width;
		bounds.h = Height;
		CPhysicsManager::GetSingleton().SetupWorld(bounds);
		CPhysicsManager::GetSingleton().ToggleDebug();
	}
	else if (pass == 4)
	{
		//fixme
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
	while (!mBalls.empty()) 
	{
		CBall *ball = mBalls.back();
		mFluid->DetachPoller(ball);
		delete ball;
		mBalls.pop_back();
	}

	delete CPhysicsManager::GetSingletonPtr();

	delete mFluid;
	mFluid = NULL;
	
	delete mOptionsWindow;
	mOptionsWindow = NULL;

	//destroy CEGUI stuff
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

	if (e.type == SDL_KEYDOWN)
	{
		//escape - back to menu screen
		if (e.key.keysym.sym == SDLK_ESCAPE) CGameManager::GetSingleton().ChangeState(new CLoadState(new CMenuState()));
		if (e.key.keysym.sym == SDLK_o) mOptionsWindow->Show();
		if (e.key.keysym.sym == SDLK_g) mFluid->InjectCheckeredData();
		if (e.key.keysym.sym == SDLK_j) mAutoInject = !mAutoInject;
		if (e.key.keysym.sym == SDLK_SPACE)
		{
			CBall *ball = new CBall(FluidSolver::Vector2(4, 3), 0.03f);
			mFluid->AttachPoller(ball);
			mBalls.push_back(ball);
		}
	}
	else
	{
		if (e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEMOTION)
		{
			int x, y;
			Uint8 state = SDL_GetMouseState(&x, &y);
			SDLMod mod = SDL_GetModState();

			if (state)
			{
				float xPos = Width * e.motion.x/mOptions.RenderResolution.x;
				float yPos = Height * (1-e.motion.y/mOptions.RenderResolution.y);
				float dx = Width * e.motion.xrel/mOptions.RenderResolution.x;
				float dy = Height * (-e.motion.yrel/mOptions.RenderResolution.y);

				float r=0, g=0, b=0;

				if (mod & KMOD_LCTRL) 
				{
					if (state & SDL_BUTTON(SDL_BUTTON_LEFT)) mFluid->Perturb(xPos, yPos, dx, dy, 0.25f);
					if (state & SDL_BUTTON(SDL_BUTTON_MIDDLE)) mFluid->Perturb(xPos, yPos, 0.2f*dx, 0.2f*dy, 0.25f);
					if (state & SDL_BUTTON(SDL_BUTTON_RIGHT)) mFluid->Perturb(xPos, yPos, 5*dx, 5*dy, 0.25f);
				}
				else
				{
					if (state & SDL_BUTTON(SDL_BUTTON_LEFT)) r += 1;
					if (state & SDL_BUTTON(SDL_BUTTON_MIDDLE)) g += 1;
					if (state & SDL_BUTTON(SDL_BUTTON_RIGHT)) b += 1;
					
					mFluid->Inject(xPos, yPos, r, g, b, 0.5f);
				}
			}
		}
	}
}
void CFreePlayState::Draw()
{
	float min=0, max=10;

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, Width, 0, Height, 0, 4);

	//reset model view matrix
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	//glDisable(GL_DEPTH_TEST);
	mFluid->Render();
	//glEnable(GL_DEPTH_TEST);

	for (BallList::iterator it = mBalls.begin(); it != mBalls.end(); ++it) {
		(*it)->Draw();
	}

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}
bool CFreePlayState::Update(float dt)
{
	if (mOptionsWindow->IsActive())
	{
		if (mOptionsWindow->IsDone())
		{
			mOptionsWindow->Hide();
			if (mOptionsWindow->IsApplied())
			{
				SetFluidOptions();
				mFluid->Init(mOptions);
				mFluid->SetColorDensities(TTC_CFG_GET(float, "densityR"), TTC_CFG_GET(float, "densityG"), TTC_CFG_GET(float, "densityB"));
			}
		}
	}
	else
	{

		//inject random thing random places
		//fixme: Make random moving thing better, and member var instead of static
		if (mAutoInject) 
		{
			static float rx=Width/2, ry=Height/2;
			static float hue = 0, dhue = 0;
			static float angle=0, dangle = 0;

			
			dangle += ((float)rand()/RAND_MAX - 0.5f);
			//if (dangle > 0.5f) dangle = 0.5f;
			//if (dangle < -0.5f) dangle = -0.5f;
			angle += dangle;
			rx += sin(dangle)*Width*dt;
			ry += cos(dangle)*Height*dt;

			if (rx > Width) rx -= Width;
			else if (rx < 0) rx += Width;
			if (ry > Height) ry -= Height;
			else if (ry < 0) ry += Height;

			dhue += ((float)rand()/RAND_MAX - 0.5f);
			if (dhue > 5) dhue = 5;
			if (dhue < -5) dhue = -5;
			hue += dhue;
			if (hue >= 360) hue -= 360;
			else if (hue < 0) hue += 360;
			int h = (int)hue;
			float f = h/60.f - h/60;

			float rr,rg,rb;
			if (h < 60) { rr = 1; rg = f; rb = 0; }
			else if (h < 120) { rr = 1-f; rg = 1; rb = 0; }
			else if (h < 180) { rr = 0; rg = 1; rb = f; }
			else if (h < 240) { rr = 0; rg = 1-f; rb = 1; }
			else if (h < 300) { rr = f; rg = 0; rb = 1; }
			else { rr = 1; rg = 0; rb = 1-f; }

			mFluid->Inject(rx, ry, 10*rr*dt, 10*rg*dt, 10*rb*dt, 0.5f);
		}

		mElapsed += dt;

		//options label
		Window *window = WindowManager::getSingleton().getWindow("FreePlay/HUD/OptionsLabel");
		if (mElapsed > 5 && mElapsed < 10)
		{
			window->setAlpha(window->getAlpha() - 0.2f*dt);
		} else if (mElapsed > 10) {
			if (window->isVisible()) window->hide();
		}

		//fluid and ball update
		mFluid->Update(dt);
		CPhysicsManager::GetSingleton().Update(dt);
		for (BallList::iterator it = mBalls.begin(); it != mBalls.end(); ++it) {
			(*it)->Update(dt);
		}

		
	}

	return true;
}


void CFreePlayState::SetFluidOptions()
{
	//config
	float resX = TTC_CFG_GET(float, "resolutionX");
	float resY = TTC_CFG_GET(float, "resolutionY");
	float fluidResX = TTC_CFG_GET(float, "fluidResolutionX");
	float fluidResY = TTC_CFG_GET(float, "fluidResolutionY");
	float viscosity = TTC_CFG_GET(float, "viscosity");

	int solverOptions;
	const string &quality = TTC_CFG_GET_STR("solverQuality");
	if (quality == "Perfect") solverOptions = RS_PERFECT;
	else if (quality == "Accurate") solverOptions = (RS_ACCURATE | RS_VORTICITY_CONFINEMENT);
	else if (quality == "Nice") solverOptions = RS_NICE;
	else if (quality == "Fast") solverOptions = RS_FAST;
	else if (quality == "Ugly") solverOptions = RS_UGLY;
	else solverOptions = RS_NONE; //error, don't do anything but pressure on render! bwa ha ha, that'll learn em

	//viscosity 0 means no diffusion happens
	//FIXME: really, this should happen in the solver
	if (viscosity == 0) solverOptions &= ~(RS_DIFFUSE_DATA | RS_DIFFUSE_VELOCITY);

	mOptions.RenderResolution = Vector(resX, resY);
	mOptions.SolverResolution = Vector(fluidResX, fluidResY);
	mOptions.Size = Vector((float)Width, (float)Height);
	mOptions.SolverOptions = solverOptions | 0&RS_DOUBLE_PRECISION;
	mOptions.Viscosity = viscosity;
}