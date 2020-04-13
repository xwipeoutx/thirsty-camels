#include "Exception.h"
#include "common.h"

using namespace TTC;

Exception::Exception(const std::string &text, const std::string &file, int line)
: mText(text), mFile(file), mLine(line)
{
}

Exception::~Exception(void)
{
}


std::string Exception::GetErrorString()
{
	std::string str = "EXCEPTION - " + mText;
	if (!mFile.empty())
		str += "\nSource File: " + mFile;
	if (mLine > 0)
		str = str + "\nSource Line: " + ToString(mLine);
	return str;
}