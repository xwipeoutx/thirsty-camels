#include "CGameBoard.h"
#include "CConfig.h"
#include "CGraphicsManager.h"
#include "CPhysicsManager.h"

using namespace TTC;
using namespace std;
using namespace FluidSolver;

CGameBoard::CGameBoard(void)
: mScore(0)
{
}

CGameBoard::~CGameBoard(void)
{
	assert(mBoardAttachments.begin() == mBoardAttachments.end());

	// Restore Matrices
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

bool CGameBoard::Initialize(int width, int height) 
{
	mWidth = width;
	mHeight = height;

	CGraphicsManager &graphicsMgr = CGraphicsManager::GetSingleton();
	mFluid = new FluidSolver::Fluid(
		graphicsMgr.GetCgContext(), 
		graphicsMgr.GetCgFragmentProfile(), 
		TTC_CFG_GET_STR("resourcePath") + "Shaders/Fluid/"
	);

	Reset(true);

	// Reset Projection Matrix - FIXME UGH
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, width, 0, height, 0, 4);

	// Reset model view matrix
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	return true; // FIXME: error handling
}

void CGameBoard::Reset(bool updateOptions) 
{
	if (updateOptions) UpdateFluidOptions();
	mFluid->Init(mOptions);
	mFluid->SetColorDensities(TTC_CFG_GET(float, "densityR"), TTC_CFG_GET(float, "densityG"), TTC_CFG_GET(float, "densityB"));
}

void CGameBoard::HandleEvent(const SDL_Event &e)
{
	for (BoardAttachmentList::iterator it = mBoardAttachments.begin(); it != mBoardAttachments.end(); ++it)
	{
		(*it)->HandleEvent(e);
	}
}

void CGameBoard::Update(float dt)
{
	assert(mFluid != NULL);

	mFluid->Update(dt);
	for (BoardAttachmentList::iterator it = mBoardAttachments.begin(); it != mBoardAttachments.end(); ++it)
	{
		(*it)->Update(dt);
	}
}
void CGameBoard::Draw()
{
	GetFluid()->Render();

	for (BoardAttachmentList::iterator it = mBoardAttachments.begin(); it != mBoardAttachments.end(); ++it)
	{
		(*it)->Draw();
	}

	// Render Physics step - useful! Happens last so it is on top
	if (CPhysicsManager::IsValid()) CPhysicsManager::GetSingleton().RenderDebug();

}

void CGameBoard::Attach(IBoardFeature *attachment)
{
	if (attachment->OnAttach(this))
	{
		mBoardAttachments.push_back( attachment);
	}
}

void CGameBoard::Detach(IBoardFeature *feature)
{
	mBoardAttachments.remove(feature);
}

void CGameBoard::DetachAll()
{
	mBoardAttachments.clear();
}

void CGameBoard::UpdateFluidOptions()
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

	// viscosity 0 means no diffusion happens
	// FIXME: really, this should happen in the solver
	if (viscosity == 0) solverOptions &= ~(RS_DIFFUSE_DATA | RS_DIFFUSE_VELOCITY);

	mOptions.RenderResolution = Vector(resX, resY);
	mOptions.SolverResolution = Vector(fluidResX, fluidResY);
	mOptions.Size = Vector((float)mWidth, (float)mHeight);
	mOptions.SolverOptions = solverOptions | 0&RS_DOUBLE_PRECISION;
	mOptions.Viscosity = viscosity;
}

void CGameBoard::AddScore(int score) 
{
	mScore += score;
}

int CGameBoard::GetScore()
{
	return mScore;
}