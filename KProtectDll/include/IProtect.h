#pragma once

#include <Windows.h>
#include <string>

class IProtect
{
public:
	virtual bool Init() = 0;

	virtual bool Protect(_In_ std::wstring Key) = 0;
	virtual bool Apply() = 0;

	virtual bool TestAhoCorsickMatch(
		_In_ const WCHAR* Path,
		_Out_ bool& Found
	) = 0;
};

