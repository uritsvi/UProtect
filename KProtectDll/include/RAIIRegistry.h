#pragma once 

#include <Windows.h>

#include <list>

class RAIIReigstryKey {

public:
	RAIIReigstryKey() = default;
	RAIIReigstryKey(
		_In_ HKEY Root,
		_In_ const WCHAR* RelativePath,
		_In_ REGSAM Access,
		_In_ bool Create);

	~RAIIReigstryKey();

	HKEY GetHANDLE();

	bool FailedToCreate();

	bool WriteValueDWORD(
		_In_ const WCHAR* ValueName,
		_In_ DWORD Value);
	bool WriteValueQWORD(
		_In_ const WCHAR* ValueName, 
		_In_ DWORD64 Value);
	bool WriteMultiWString(
		_In_ const WCHAR* ValueName, 
		_In_ std::list<std::wstring> String);
	bool WriteValue(
		_In_ const WCHAR* ValueName,
		_In_ const void* Buffer,
		_In_ int Size);

	bool ReadMultyStringValue(
		const WCHAR* Name, 
		std::list<std::wstring>& Paths);

	bool DeleteAllValue();
	bool DeleteValue(_In_ const WCHAR* Name);

private:
	HKEY m_Key;
	bool m_Failed;
};