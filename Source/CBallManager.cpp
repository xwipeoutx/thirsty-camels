#include "CBallManager.h"
#include "CGameBoard.h"

#include "CBall.h"
#include "CSpriteFactory.h"

using namespace TTC;

CBallManager::CBallManager()
: IBoardFeature()
{
	CSpriteFactory::GetSingleton().Load("ball", "Cactus1", "Cactus1.png", 1, 1);
}

CBallManager::~CBallManager(void)
{
	while (!mBalls.empty()) 
	{
		CBall *ball = mBalls.back();
		GetGameBoard()->GetFluid()->DetachPoller(ball);
		delete ball;
		mBalls.pop_back();
	}
}

void CBallManager::AddBall(float x, float y)
{
	CBall *ball = new CBall(FluidSolver::Vector2(x, y), 0.08f);
	GetGameBoard()->GetFluid()->AttachPoller(ball);
	mBalls.push_back(ball);
}

bool CBallManager::Update(float dt)
{
	
	//kill any dead balls
	BallList::iterator it = mBalls.begin();
	while(it != mBalls.end())
	{
		if ((*it)->GetDying())
		{
			CBall *die = *it;
			GetGameBoard()->GetFluid()->DetachPoller(die);
			it = mBalls.erase(it);
			delete die;
		}
		else
		{
			++it;
		}
	}

	CPhysicsManager::GetSingleton().Update(dt);
	for (it = mBalls.begin(); it != mBalls.end(); ++it) {
		(*it)->Update(dt);
	}
	return true;
}

void CBallManager::Draw()
{
	for (BallList::iterator it = mBalls.begin(); it != mBalls.end(); ++it) {
		(*it)->Draw();
	}
}