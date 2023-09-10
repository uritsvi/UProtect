#pragma once 

#include "AhoCorasickInterface.h"
#include "IProtect.h"
#include "..\..\Common\Common.h"

class FilePaths : public IProtect {
public:
	FilePaths();
	~FilePaths();

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