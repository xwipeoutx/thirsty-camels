#pragma once

#include <fstream>
#include "CSingleton.h"

namespace TTC
{
	enum LogLevel
	{
		LL_DEBUG=0,
		LL_INFO=1,
		LL_WARN=2,
		LL_ERROR=3,
		LL_FATAL=4
	};

	class CLog : public CSingleton<CLog>
	{
	public:
		CLog(void);
		~CLog(void);

		static inline CLog& Get()
		{
			return GetSingleton();
		}

		void SetLogLevel(const std::string &logLevel);
		void SetLogLevel(LogLevel logLevel);

		//convenience functions
		template <class _T>
		inline void Debug(_T msg){Log(LL_DEBUG, msg);}
		template <class _T>
		inline void Info(_T msg){Log(LL_INFO, msg);}
		template <class _T>
		inline void Warn(_T msg){Log(LL_WARN, msg);}
		template <class _T>
		inline void Error(_T msg){Log(LL_ERROR, msg);}
		template <class _T>
		inline void Fatal(_T msg){Log(LL_FATAL, msg);}

		template <class _T>
		inline void Log(LogLevel level, _T msg)
		{
			if (level >= mLogLevel)
			{
				if (level == LL_DEBUG) mLogFile <<      "[DEBUG] ";
				else if (level == LL_INFO) mLogFile <<  "[INFO]  ";
				else if (level == LL_WARN) mLogFile <<  "[WARN]  ";
				else if (level == LL_ERROR) mLogFile << "[ERROR] ";
				else if (level == LL_FATAL) mLogFile << "[FATAL] ";
				mLogFile << msg << std::endl;
				mLogFile.flush();
			}
		}


	private:
		LogLevel mLogLevel;
		std::ofstream mLogFile;

	};
}