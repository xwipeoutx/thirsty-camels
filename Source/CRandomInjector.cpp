#include "CRandomInjector.h"
#include "FluidSolver.h"
#include <ctime>
#include <stdlib.h>
#include "CGameBoard.h"
#include <cmath>

using namespace TTC;
using namespace FluidSolver;

CRandomInjector::CRandomInjector(float size, float intensity)
: IBoardFeature(), mHue(0), mHueDelta(0), mAngle(0), mSize(size), mIntensity(intensity), mX(0), mY(0)
{
}

CRandomInjector::~CRandomInjector(void)
{
}

bool CRandomInjector::OnAttach(CGameBoard *gameBoard)
{
	IBoardFeature::OnAttach(gameBoard);

	Vector size = GetGameBoard()->GetFluid()->GetSize();
	mX = randf() * size.x;
	mY = randf() * size.y;

	return true;
}

bool CRandomInjector::Update(float dt)
{
	mAngle += ((float)rand()/RAND_MAX - 0.5f);
	const Vector2 &size = GetGameBoard()->GetFluid()->GetSize();

	mX += sin(mAngle) * size.x * dt;
	mY += cos(mAngle) * size.y * dt;

	// FIXME: Also kinda dodgy, should only ever happen the first time though
	while (mX > size.x) mX -= size.x;
	while (mX < 0) mX += size.x;
	while (mY > size.y) mY -= size.y;
	while (mY < 0) mY += size.y;

	mHueDelta += ((float)rand()/RAND_MAX - 0.5f);
	if (mHueDelta > 5) mHueDelta = 5;
	if (mHueDelta < -5) mHueDelta = -5;
	mHue += mHueDelta;
	if (mHue >= 360) mHue -= 360;
	else if (mHue < 0) mHue += 360;

	//convert hue to rgb
	int h = (int)mHue;
	float f = h/60.f - h/60;
	float r,g,b;
	if (h < 60) 
	{ r = 1; g = f; b = 0; }
	else if (h < 120) 
	{ r = 1-f; g = 1; b = 0; }
	else if (h < 180) 
	{ r = 0; g = 1; b = f; }
	else if (h < 240) 
	{ r = 0; g = 1-f; b = 1; }
	else if (h < 300) 
	{ r = f; g = 0; b = 1; }
	else 
	{ r = 1; g = 0; b = 1-f; }

	GetGameBoard()->GetFluid()->Inject(mX, mY, mIntensity*r*dt, mIntensity*g*dt, mIntensity*b*dt, mSize);

	return true;
}