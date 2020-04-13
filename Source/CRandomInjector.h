#pragma once

#include "IBoardFeature.h"

namespace TTC
{
	class CRandomInjector : public IBoardFeature
	{
	public:
		CRandomInjector(float size=0.5f, float intensity=10.f);
		~CRandomInjector(void);

		virtual bool OnAttach(CGameBoard *gameBoard);

		virtual bool Update(float dt);
		virtual void Draw(){/*FIXME*/}

	private:
		float mWidth, mHeight;
		float mX, mY;
		float mHue, mHueDelta;
		float mAngle;
		float mSize, mIntensity;
	};
}