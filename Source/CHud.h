#pragma once

#include "common.h"
#include "CSingleton.h"

namespace TTC
{
	class CHud : public CSingleton<CHud>
	{
	public:
		CHud(void);
		~CHud(void);

	private:
	};
}
