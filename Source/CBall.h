#pragma once

#include "CActor.h"
#include "CPhysicsManager.h"
#include "FluidSolver.h"

#include "common.h"

namespace TTC
{
	class CBall : public CActor, public FluidSolver::IVelocityPoller
	{
	public:
		CBall(FluidSolver::Vector2 position, float radius);
		virtual ~CBall(void);

		virtual bool Update(float dt);
		virtual void Draw();

	void UpdateVelocity(const FluidSolver::Vector &v)
	{
		mFluidVelocity = v;
	}

	protected:
		float mSpeedUp;
		int mCallList;

		b2Body *mBody;
		float mRadius;

		FluidSolver::Vector mVelocity;
		FluidSolver::Vector mFluidVelocity;
	};
};
