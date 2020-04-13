#include "CFluidInjector.h"
#include "FluidSolver.h"
#include "CGameBoard.h"

using namespace TTC;
using namespace FluidSolver;

CFluidInjector::CFluidInjector()
: IBoardFeature()
{
}

CFluidInjector::~CFluidInjector(void)
{
}

bool CFluidInjector::OnAttach(CGameBoard *gameBoard)
{
	IBoardFeature::OnAttach(gameBoard);
	return true;
}


void CFluidInjector::HandleEvent(const SDL_Event &e)
{
	//FIXME: Split up injection and perturbation
	if (e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEMOTION)
	{
		int x, y;
		Uint8 state = SDL_GetMouseState(&x, &y);
		SDLMod mod = SDL_GetModState();

		if (state)
		{
			const FluidSolver::FluidOptions &options = GetGameBoard()->GetOptions();
			Fluid *fluid = GetGameBoard()->GetFluid();
			float width = fluid->GetSize().x;
			float height = fluid->GetSize().y;

			float xPos = width * e.motion.x/options.RenderResolution.x;
			float yPos = height * (1-e.motion.y/options.RenderResolution.y);
			float dx = width * e.motion.xrel/options.RenderResolution.x;
			float dy = height * (-e.motion.yrel/options.RenderResolution.y);

			float r=0, g=0, b=0;

			if (mod & KMOD_LCTRL) 
			{
				if (state & SDL_BUTTON(SDL_BUTTON_LEFT)) fluid->Perturb(xPos, yPos, dx, dy, 0.25f);
				if (state & SDL_BUTTON(SDL_BUTTON_MIDDLE)) fluid->Perturb(xPos, yPos, 0.2f*dx, 0.2f*dy, 0.25f);
				if (state & SDL_BUTTON(SDL_BUTTON_RIGHT)) fluid->Perturb(xPos, yPos, 5*dx, 5*dy, 0.25f);
			}
			else
			{
				if (state & SDL_BUTTON(SDL_BUTTON_LEFT)) r += 1;
				if (state & SDL_BUTTON(SDL_BUTTON_MIDDLE)) g += 1;
				if (state & SDL_BUTTON(SDL_BUTTON_RIGHT)) b += 1;
				
				fluid->Inject(xPos, yPos, r, g, b, 0.5f);
			}
		}
	}
}

bool CFluidInjector::Update(float dt)
{
	return true;
}