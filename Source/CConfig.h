#pragma once

#include "common.h"
#include "CSingleton.h"
#include <string>
#include <sstream >
#include <map>
#include <vector>

typedef std::map<std::string, std::string> ConfigMap;

//fixme MACROS AM TEH BAD
#define TTC_CFG_GET_PATH(_TYPE) CConfig::GetSingleton().GetFullPathTo(_TYPE)
#define TTC_CFG_GET(_T, _KEY) CConfig::GetSingleton().Get<_T>(_KEY)
#define TTC_CFG_GET_STR(_KEY) CConfig::GetSingleton().GetString(_KEY)
#define TTC_CFG_SET(_KEY, _VALUE) CConfig::GetSingleton().Set(_KEY, _VALUE)

namespace TTC
{
	class CConfig : public CSingleton<CConfig>
	{
	public:
		CConfig(void);
		~CConfig(void);

		void Load(std::string filename);
		void LoadDefaults();
		bool Save(std::string filename);
		const ConfigMap &GetAllOptions() const
		{
			return mConfigMap;
		}

		std::string GetFullPathTo(std::string resourceType);
		
		std::string GetString(std::string key)
		{
			ConfigMap::iterator it = mConfigMap.find(key);
			if (it == mConfigMap.end()) {
				throw TTC::Exception("Error loading config option: " + key);
			}
			return it->second;
		}

		template <class _T>
		_T Get(std::string key)
		{
			_T ret;
			ConfigMap::iterator it = mConfigMap.find(key);
			if (it == mConfigMap.end()) {
				throw TTC::Exception("Error loading config option: " + key);
			}
			std::stringstream stream(it->second);
			stream >> ret;
			return ret;
		}
		
		template <class _T>
		void Set(std::string key, _T value)
		{
			std::stringstream valueStream;
			valueStream << value;

			mConfigMap[key] = valueStream.str();
		}
		
	private:
		ConfigMap mConfigMap;

		bool mLoaded;
	};
}