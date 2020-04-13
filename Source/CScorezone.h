#pragma once

#include "IPhysicsObject.h"
#include "IBoardFeature.h"
#include "CPhysicsManager.h"

namespace TTC
{
	class CScorezone : public IBoardFeature, public IPhysicsObject
	{
	public:
		CScorezone();
		~CScorezone(void);

		virtual bool Update(float dt);
		virtual void Draw(){/*FIXME*/}
		virtual void Collide(IPhysicsObject *other, const CPhysicsManager::ContactPoint &cp, int pos);
	};
};