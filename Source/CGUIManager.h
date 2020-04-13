#pragma once
#include "CSingleton.h"
#include "common.h"

namespace TTC
{
	class CGUIManager : public CSingleton<CGUIManager>
	{
	public:
		CGUIManager(void);
		~CGUIManager(void);

		void Init();
		void Destroy();
		void Resize();

		void HandleEvent(SDL_Event &e);
		void Update(float dt);
		void Draw();
	protected:
		void HandleMouseButtonDown(Uint8 &button);
		void HandleMouseButtonUp(Uint8 &button);
		void HandleKeyDown(SDL_keysym &button);
		void HandleKeyUp(SDL_keysym &button);

		CEGUI::System *mSystem;
		CEGUI::Renderer *mRenderer;
	};
};