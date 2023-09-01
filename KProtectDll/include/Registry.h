#pragma once 

#include <Windows.h>

#include <memory>
#include <sal.h>
#include <list>

#include "AhoCorasickInterface.h"

class Registry {
	
public:
	bool Protect(std::wstring Key);
	bool Apply();
	bool Save();
	bool Load();


private:

};

