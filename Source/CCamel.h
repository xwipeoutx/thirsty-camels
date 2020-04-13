#pragma once

#include "IBoardFeature.h"
#include "CActor.h"
#include "CPhysicsManager.h"
#include "FluidSolver.h"

#include "common.h"

namespace TTC
{
	class CSprite;

	class CCamel : public CActor, public IBoardFeature
	{
	public:
		CCamel(FluidSolver::Vector2 position, int direction, float scale, SDLKey key);
		virtual ~CCamel(void);

		virtual void HandleEvent(const SDL_Event &e);
		virtual bool Update(float dt);
		virtual void Draw();
		virtual void Collide(IPhysicsObject *other, const CPhysicsManager::ContactPoint &cp, int pos);

		inline int GetScore() { return mScore; }

	protected:
		FluidSolver::Vector2 mPosition;
		int mDirection;
		float mScale;
		SDLKey mKey;
		CSprite *mSprite;

		int mScore;

		bool mIsDrinking;
		float mPower;
	};
};
