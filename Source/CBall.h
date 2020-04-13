#pragma once

#include "CActor.h"
#include "CPhysicsManager.h"
#include "FluidSolver.h"

#include "common.h"

namespace TTC
{
	class CSprite;

	class CBall : public CActor, public FluidSolver::IVelocityPoller
	{
	public:
		CBall(FluidSolver::Vector2 position, float radius);
		virtual ~CBall(void);

		virtual bool Update(float dt);
		virtual void Draw();
		virtual void Collide(IPhysicsObject *other, const CPhysicsManager::ContactPoint &cp, int pos);

	void UpdateVelocity(const FluidSolver::Vector &v)
	{
		mFluidVelocity = v;
	}

	protected:
		float mSpeedUp;

		float mRadius;

		FluidSolver::Vector mVelocity;
		FluidSolver::Vector mFluidVelocity;

		CSprite *mSprite;
	};
};
