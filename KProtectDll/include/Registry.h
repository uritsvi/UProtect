#pragma once 

#include <Windows.h>

#include <memory>
#include <sal.h>
#include <list>

#include "AhoCorasickInterface.h"
#include "IProtect.h"

class Registry : public IProtect{
	
public:
	Registry();
	~Registry();

	bool Init();

	bool Protect(_In_ std::wstring Key);
	bool Apply();

	bool TestAhoCorsickMatch(
		_In_ const WCHAR* Path,
		_Out_ bool& Found
	);

private:
	AhoCorasick m_AhoCorasick;
};

