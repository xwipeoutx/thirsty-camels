#pragma once

#include "IPhysicsObject.h"
#include "IBoardFeature.h"
#include "CPhysicsManager.h"

namespace TTC
{
	class CBall;

	class CBallManager : public IBoardFeature
	{
	public:
		CBallManager();
		~CBallManager(void);

		virtual void AddBall(float x, float y);

		virtual bool Update(float dt);
		virtual void Draw();

	protected:
		typedef std::list<CBall *> BallList;
		BallList mBalls;
	};
};