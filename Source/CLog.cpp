#include "CLog.h"

using namespace TTC;
using namespace std;

CLog::CLog(void)
{
#ifdef _DEBUG
	SetLogLevel(LL_INFO);
#else
	SetLogLevel(LL_WARN);
#endif
	mLogFile.open("app.log");
	Info("Log File Opened");
}

CLog::~CLog(void)
{
	mLogFile.close();
}

void CLog::SetLogLevel(const std::string &logLevel)
{
	if (logLevel.compare("debug") == 0) SetLogLevel(LL_DEBUG);
	else if (logLevel.compare("info") == 0) SetLogLevel(LL_INFO);
	else if (logLevel.compare("warn") == 0) SetLogLevel(LL_WARN);
	else if (logLevel.compare("error") == 0) SetLogLevel(LL_ERROR);
	else if (logLevel.compare("fatal") == 0) SetLogLevel(LL_FATAL);
}
void CLog::SetLogLevel(LogLevel logLevel)
{
	mLogLevel = logLevel;
}