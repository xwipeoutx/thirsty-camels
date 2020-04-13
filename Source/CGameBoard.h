#pragma once

#include "FluidSolver.h"
#include "IBoardFeature.h"
#include "common.h"

namespace TTC 
{

	class CGameBoard
	{
	public:
		CGameBoard(void);
		~CGameBoard(void);

		bool Initialize(int width, int height);
		void Reset(bool updateOptions = true);

		FluidSolver::Fluid *GetFluid() const { return mFluid; }
		const FluidSolver::FluidOptions &GetOptions() const { return mOptions; }

		void Update(float dt);
		void Draw();
		
		virtual void HandleEvent(const SDL_Event &e);
		void Attach(IBoardFeature *);
		void Detach(IBoardFeature *);
		void DetachAll();

		void AddScore(int score);
		int GetScore();

	protected:
		void UpdateFluidOptions();

	private:
		int mWidth;
		int mHeight;

		FluidSolver::Fluid *mFluid;
		FluidSolver::FluidOptions mOptions;

		typedef std::list< IBoardFeature * > BoardAttachmentList;
		BoardAttachmentList mBoardAttachments;

		int mScore;
	};
}