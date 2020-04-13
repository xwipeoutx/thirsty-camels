#pragma once

#include <cassert>

template<typename T>
class CSingleton
{
	static T* msSingleton;
	public:
		CSingleton()
		{
			assert(!msSingleton);
			//use a cunning trick to get the singleton pointing to the start of the whole, 
			//rather than the start of the Singleton part of the object
			int offset = (int)(T*)1 - (int)(CSingleton <T>*)(T*)1;
			msSingleton = (T*)((int)this + offset);
		}
		
		~CSingleton()
		{
			assert(msSingleton);
			msSingleton = 0;
		}
		
		static T& GetSingleton()
		{
			assert(msSingleton);
			return *msSingleton;
		}
		
		static T* GetSingletonPtr()
		{
			assert(msSingleton);
			return msSingleton;
		}
		static bool IsValid()
		{
			return msSingleton != NULL;
		}

};

template <typename T> T* CSingleton <T>::msSingleton = 0;