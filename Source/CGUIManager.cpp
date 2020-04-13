#include "CGUIManager.h"
#include "CLog.h"
#include "CGameWindow.h"
#include "CConfig.h"

using namespace TTC;
CGUIManager::CGUIManager(void)
{
	mRenderer = NULL;
	mSystem = NULL;
}

CGUIManager::~CGUIManager(void)
{
	if (mRenderer) mRenderer = NULL;
	if (mSystem) mSystem = NULL;
}

void CGUIManager::Init()
{
	try 
	{
		mRenderer = new CEGUI::OpenGLRenderer(0, CGameWindow::GetSingleton().GetWidth(), CGameWindow::GetSingleton().GetHeight());
		mSystem = new CEGUI::System(mRenderer);
		SDL_ShowCursor(SDL_DISABLE);
		SDL_EnableUNICODE(1);
		//SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

		CEGUI::DefaultResourceProvider *rp = static_cast<CEGUI::DefaultResourceProvider*>
			(mSystem->getResourceProvider());

		std::string guiPath = TTC_CFG_GET_STR("resourcePath") + "GUI/";
		rp->setResourceGroupDirectory("schemes", guiPath + "schemes/");
		rp->setResourceGroupDirectory("imagesets", guiPath + "imagesets/");
		rp->setResourceGroupDirectory("fonts", guiPath + "fonts/");
		rp->setResourceGroupDirectory("layouts", guiPath + "layouts/");
		rp->setResourceGroupDirectory("looknfeels", guiPath + "looknfeels/");

		CEGUI::Scheme::setDefaultResourceGroup("schemes");
		CEGUI::Imageset::setDefaultResourceGroup("imagesets");
		CEGUI::Font::setDefaultResourceGroup("fonts");
		CEGUI::WindowManager::setDefaultResourceGroup("layouts");
		CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");

		CEGUI::SchemeManager::getSingleton().loadScheme( "TaharezLook.scheme" );
		CEGUI::SchemeManager::getSingleton().loadScheme( "ThirstyThirstyCamels.scheme" );

		mSystem->setDefaultFont( "GoudyStout-Normal" );
		mSystem->setDefaultMouseCursor( "TaharezLook", "MouseArrow" );

		mSystem->setDefaultTooltip( "TaharezLook/Tooltip" );


	} 
	catch (CEGUI::Exception &e)
	{
		throw Exception(std::string(e.getMessage().c_str()));
	}
	CLog::Get().Info("GUI System Initialised");
}

void CGUIManager::Destroy()
{
	delete mSystem;
	mSystem = NULL;
	delete mRenderer;
	mRenderer = NULL;
	CLog::Get().Info("GUI System Destroyed");
}

void CGUIManager::HandleEvent(SDL_Event &e)
{
	switch (e.type)
	{
	case SDL_MOUSEMOTION:
		mSystem->injectMousePosition(
			static_cast<float>(e.motion.x),
			static_cast<float>(e.motion.y) );
		break;
	case SDL_MOUSEBUTTONDOWN:
		HandleMouseButtonDown(e.button.button);
		break;
	case SDL_MOUSEBUTTONUP:
		HandleMouseButtonUp(e.button.button);
		break;
	case SDL_KEYDOWN:
		HandleKeyDown(e.key.keysym);
		break;
	case SDL_KEYUP:
		HandleKeyUp(e.key.keysym);
		break;
	default:
		break;
	}
}

void CGUIManager::Update(float dt)
{
	mSystem->injectTimePulse(dt);
}

void CGUIManager::Draw()
{
	mSystem->renderGUI();
}

void CGUIManager::HandleMouseButtonDown(Uint8 &button)
{
	switch (button)
	{
	case SDL_BUTTON_LEFT:
		mSystem->injectMouseButtonDown(CEGUI::LeftButton);
		break;
	case SDL_BUTTON_MIDDLE:
		mSystem->injectMouseButtonDown(CEGUI::MiddleButton);
		break;
	case SDL_BUTTON_RIGHT:
		mSystem->injectMouseButtonDown(CEGUI::RightButton);
		break;
		
	case SDL_BUTTON_WHEELDOWN:
		mSystem->injectMouseWheelChange( -1 );
		break;
	case SDL_BUTTON_WHEELUP:
		mSystem->injectMouseWheelChange( +1 );
		break;
	}
}

void CGUIManager::HandleMouseButtonUp(Uint8 &button)
{
	switch (button)
	{
	case SDL_BUTTON_LEFT:
		mSystem->injectMouseButtonUp(CEGUI::LeftButton);
		break;
	case SDL_BUTTON_MIDDLE:
		mSystem->injectMouseButtonUp(CEGUI::MiddleButton);
		break;
	case SDL_BUTTON_RIGHT:
		mSystem->injectMouseButtonUp(CEGUI::RightButton);
		break;
	}
}

void CGUIManager::HandleKeyDown(SDL_keysym &sym)
{
	mSystem->injectKeyDown(sym.scancode);
	if (sym.unicode != 0) mSystem->injectChar(sym.unicode);
}
void CGUIManager::HandleKeyUp(SDL_keysym &sym)
{
	mSystem->injectKeyUp(sym.scancode);
}
