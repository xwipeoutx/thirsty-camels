#pragma once

#include <string>

namespace TTC
{
	class Exception
	{
	public:
		Exception(const std::string &text, const std::string &file="", int line=0);
		~Exception(void);

		std::string GetErrorString();

		std::string mText;
		std::string mFile;
		int mLine;
	};
}