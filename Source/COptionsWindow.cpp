#include "COptionsWindow.h"
#include "CGameManager.h"
#include "CConfig.h"
#include "CEGUI.h"

#include "CGUIManager.h"

#include <string>
#include <sstream>

using namespace std;
using namespace TTC;
using namespace CEGUI;

COptionsWindow::COptionsWindow(bool enableResolution, Window *parentWindow)
: mEnableResolution(enableResolution), mDone(false), mApplied(false), 
mParentWindow(parentWindow)
{
	WindowManager::getSingleton().loadWindowLayout( "optionslayout.xml" );

	mWindow = static_cast<PushButton*>(WindowManager::getSingleton().getWindow("Options"));
	mOkButton = static_cast<PushButton*>(WindowManager::getSingleton().getWindow("Options/OkButton"));
	mCancelButton = static_cast<PushButton*>(WindowManager::getSingleton().getWindow("Options/CancelButton"));
	mResolutionLabel = WindowManager::getSingleton().getWindow("Options/ControlContainer/ResolutionLabel");
	mResolution = static_cast<Combobox*>(WindowManager::getSingleton().getWindow("Options/ControlContainer/Resolution"));
	mFluidResolution = static_cast<Combobox*>(WindowManager::getSingleton().getWindow("Options/ControlContainer/FluidResolution"));
	mSolverQuality = static_cast<Combobox*>(WindowManager::getSingleton().getWindow("Options/ControlContainer/SolverQuality"));
	mViscosity = WindowManager::getSingleton().getWindow("Options/ControlContainer/Viscosity");
	mDensityR = WindowManager::getSingleton().getWindow("Options/ControlContainer/DensityR");
	mDensityG = WindowManager::getSingleton().getWindow("Options/ControlContainer/DensityG");
	mDensityB = WindowManager::getSingleton().getWindow("Options/ControlContainer/DensityB");

	Init();
	mParentWindow->addChildWindow(mWindow);
	mWindow->hide();
}

COptionsWindow::~COptionsWindow(void)
{
	mParentWindow->removeChildWindow(mWindow);
	if (WindowManager::getSingleton().isWindowPresent("Options"))
		WindowManager::getSingleton().destroyWindow("Options");
}

void COptionsWindow::Init()
{
	//attach events
	mOkButton->subscribeEvent(PushButton::EventClicked, Event::Subscriber(&COptionsWindow::onOkButtonClicked, this));
	mCancelButton->subscribeEvent(PushButton::EventClicked, Event::Subscriber(&COptionsWindow::onCancelButtonClicked, this));

	mResolution->setText(TTC_CFG_GET_STR("resolutionX") + " x " + TTC_CFG_GET_STR("resolutionY"));
	if (mEnableResolution)
	{
		mResolution->addItem(new ListboxTextItem("640 x 480"));
		mResolution->addItem(new ListboxTextItem("800 x 600"));
		mResolution->addItem(new ListboxTextItem("1024 x 768"));
		mResolution->addItem(new ListboxTextItem("1280 x 1024"));
	}
	else
	{
		mResolutionLabel->hide();
		mResolution->hide();
	}
	
	mFluidResolution->addItem(new ListboxTextItem("200 x 150"));
	mFluidResolution->addItem(new ListboxTextItem("400 x 300"));
	mFluidResolution->addItem(new ListboxTextItem("640 x 480"));
	mFluidResolution->addItem(new ListboxTextItem("800 x 600"));
	mFluidResolution->addItem(new ListboxTextItem("1024 x 768"));
	mFluidResolution->addItem(new ListboxTextItem("1280 x 1024"));
	mFluidResolution->setText(TTC_CFG_GET_STR("fluidResolutionX") + " x " + TTC_CFG_GET_STR("fluidResolutionY"));
	
	Combobox *solverQualityCombo = static_cast<Combobox*>(WindowManager::getSingleton().getWindow("Options/ControlContainer/SolverQuality"));
	mSolverQuality->addItem(new ListboxTextItem("Perfect"));
	mSolverQuality->addItem(new ListboxTextItem("Accurate"));
	mSolverQuality->addItem(new ListboxTextItem("Nice"));
	mSolverQuality->addItem(new ListboxTextItem("Fast"));
	mSolverQuality->addItem(new ListboxTextItem("Ugly"));
	mSolverQuality->setText(TTC_CFG_GET_STR("solverQuality"));

	mViscosity->setText(TTC_CFG_GET_STR("viscosity"));
	
	mDensityR->setText(TTC_CFG_GET_STR("densityR"));
	mDensityG->setText(TTC_CFG_GET_STR("densityG"));
	mDensityB->setText(TTC_CFG_GET_STR("densityB"));
}

void COptionsWindow::Show()
{
	mDone = false;
	mWindow->show();
	mWindow->activate();
}

void COptionsWindow::Hide()
{
	mWindow->hide();
}

bool COptionsWindow::onOkButtonClicked(const CEGUI::EventArgs &e)
{
	//some place holder variables for sstream stuff
	int intOpt1, intOpt2;
	float fltOpt1;
	string strOpt1;
	stringstream stream;

	//resolution
	if (mEnableResolution)
	{
		stream.clear();
		stream.str(GetFromWindow("Resolution"));
		//stringstream resolutionStream(GetFromWindow("Resolution"));
		stream >> intOpt1 >> strOpt1 >> intOpt2;
		TTC_CFG_SET("resolutionX", intOpt1);
		TTC_CFG_SET("resolutionY", intOpt2);
	}
	
	//fluid resolution
	stream.clear();
	stream.str(GetFromWindow("FluidResolution"));
	stream >> intOpt1 >> strOpt1 >> intOpt2;
	TTC_CFG_SET("fluidResolutionX", intOpt1);
	TTC_CFG_SET("fluidResolutionY", intOpt2);

	stream.clear();
	stream.str(GetFromWindow("SolverQuality"));
	stream >> strOpt1;
	TTC_CFG_SET("solverQuality", strOpt1);

	stream.clear();
	stream.str(GetFromWindow("Viscosity"));
	stream >> fltOpt1;
	TTC_CFG_SET("viscosity", fltOpt1);

	stream.clear();
	stream.str(GetFromWindow("DensityR"));
	stream >> fltOpt1;
	TTC_CFG_SET("densityR", fltOpt1);

	stream.clear();
	stream.str(GetFromWindow("DensityG"));
	stream >> fltOpt1;
	TTC_CFG_SET("densityG", fltOpt1);

	stream.clear();
	stream.str(GetFromWindow("DensityB"));
	stream >> fltOpt1;
	TTC_CFG_SET("densityB", fltOpt1);

	CConfig::GetSingleton().Save("config.ini");
	if (mEnableResolution) 
	{
		CGameManager::GetSingleton().Restart();
	}
	else 
	{
		mDone = mApplied = true;
	}
	return true;
}

bool COptionsWindow::onCancelButtonClicked(const CEGUI::EventArgs &e)
{
	mDone = true;
	mApplied = false;
	return true;
}

const char* COptionsWindow::GetFromWindow(std::string key)
{
	return WindowManager::getSingleton().getWindow("Options/ControlContainer/" + key)->getText().c_str();
}