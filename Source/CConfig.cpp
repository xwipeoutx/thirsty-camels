#include "CConfig.h"
#include "CLog.h"
#include <fstream>

using namespace TTC;
using namespace std;

CConfig::CConfig(void)
: mLoaded(false)
{

}

CConfig::~CConfig(void)
{

}

void CConfig::LoadDefaults()
{
	mConfigMap["resolutionX"] = "800";
	mConfigMap["resolutionY"] = "600";
	mConfigMap["fluidResolutionX"] = "400";
	mConfigMap["fluidResolutionY"] = "300";

	mConfigMap["solverQuality"] = "Fast";

	mConfigMap["viscosity"] = "0";

	mConfigMap["densityR"] = "1";
	mConfigMap["densityG"] = "0";
	mConfigMap["densityB"] = "-1";

#ifdef _DEBUG
	mConfigMap["logLevel"] = "info";
	mConfigMap["resourcePath"] = "../../../../Resources/";
	mConfigMap["windowTitle"] = "Thirsty,Thirsty Camels! [DEBUG]";
#else
	mConfigMap["logLevel"] = "warn";
	mConfigMap["resourcePath"] = "./Resources/";
	mConfigMap["windowTitle"] = "Thirsty, Thirsty Camels";
#endif
}

void CConfig::Load(std::string filename)
{
	LoadDefaults();

	fstream configStream;
	configStream.open(filename.c_str(), ios_base::in);
	CLog::Get().Info("Loading Config File from: " + filename);

	if (configStream.is_open())
	{
		std::string key, value;

		std::string line;
		while (getline(configStream, line))
		{
			size_t pos, startPos, endPos;
			
			pos = line.find_first_of("=");

			key = line.substr(0, pos);
			startPos = key.find_first_not_of(" \t");
			endPos = key.find_last_not_of(" \t");
			if(( string::npos == startPos ) || ( string::npos == endPos)) key = "";
			else key = key.substr( startPos, endPos - startPos + 1 );

			value = line.substr(pos+1);
			startPos = value.find_first_not_of(" \t");
			endPos = value.find_last_not_of(" \t");
			if(( string::npos == startPos ) || ( string::npos == endPos)) value = "";
			else value = value.substr( startPos, endPos - startPos + 1 );

			if (!(key.empty())) {
				mConfigMap[key] = value;
				CLog::Get().Debug("CFG: " + key + " = " + value);
			}

			int blarg=42;
		}

		configStream.close();
	}
	mLoaded = true;
}

bool CConfig::Save(std::string filename)
{
	if (!mLoaded) throw Exception("Error: Config hasn't been loaded yet!");

	fstream configStream;
	configStream.open(filename.c_str(), ios_base::out);

	if (!configStream.is_open()) {
		CLog::Get().Warn("Failed Saved config file to: " + filename);
		return false;
	}

	ConfigMap::iterator it = mConfigMap.begin();
	for (it; it != mConfigMap.end(); ++it)
	{
		configStream << it->first << " = " << it->second<< std::endl;
	}
	configStream.flush();
	configStream.close();
	CLog::Get().Info("Saved config file to: " + filename);
	return true;

}