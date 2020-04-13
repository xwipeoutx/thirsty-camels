#pragma once

#include "IBoardFeature.h"

namespace TTC
{
	class CFluidInjector : public IBoardFeature
	{
	public:
		CFluidInjector();
		~CFluidInjector(void);

		virtual bool OnAttach(CGameBoard *gameBoard);

		virtual void HandleEvent(const SDL_Event &e);
		virtual bool Update(float dt);
		virtual void Draw(){/*FIXME*/}
	};
}